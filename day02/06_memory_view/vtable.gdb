# =========================================================================
# vtable.gdb — helpers for a class on C++ object layout, vtables and RTTI
#              (Itanium C++ ABI: GCC / Clang on Linux, 64-bit)
#
#   gdb -q -x vtable.gdb ./vtble_demo
#
# Commands defined here:
#   vtable <obj|ptr>      raw vtable dump: ABI header + every slot, by symbol
#   layout <obj|ptr>      runtime byte map of the DYNAMIC type: every base
#                         subobject (virtual ones too), vptr, member, hole
#   rtti   <obj|ptr|Type> the inheritance tree as recorded in typeinfo
#   subclasses <Type>     every class in the program that derives from Type
#
# Why not just "info vtbl" and "ptype /o"?  Both are driven by DWARF, so:
#   - info vtbl can omit slots it has no declaration for (try it on `io`:
#     IOStream's destructor slots are missing) and needs a complete type;
#   - ptype /o hides the vptr and cannot place virtual bases (it calls the
#     virtual Stream inside IOStream "16-byte padding").
# The helpers below read the real bytes and ELF symbols instead, so they
# also work on libstdc++ objects that have no debug info at all.
# =========================================================================

set confirm off
set pagination off
set print pretty off
set print asm-demangle on
set disable-randomization on

python
import re
import gdb

_PTR = 8


def _qword(addr):
    inf = gdb.selected_inferior()
    return int.from_bytes(bytes(inf.read_memory(addr, _PTR)), "little")


def _signed(v):
    return v - (1 << 64) if v & (1 << 63) else v


_SYM_RE = re.compile(r"^(.*?)(?: \+ (\d+))? in section (\S+)(?: of (.*))?$")


def _sym(addr):
    """(name, offset, section) for an address, from ELF symbols, or None."""
    if addr == 0:
        return None
    try:
        out = gdb.execute("info symbol 0x%x" % addr, to_string=True).strip()
    except gdb.error:
        return None
    m = _SYM_RE.match(out.splitlines()[0]) if out else None
    if not m:
        return None
    return m.group(1), int(m.group(2) or 0), m.group(3)


def _is_code(addr):
    s = _sym(addr)
    return s is not None and s[2].startswith(".text")


def _is_typeinfo(addr):
    s = _sym(addr)
    return s is not None and s[0].startswith("typeinfo for") and s[1] == 0


def _symtext(addr):
    s = _sym(addr)
    if s is None:
        return "0x%x" % addr
    return "%s+%d" % (s[0], s[1]) if s[1] else s[0]


def _object_address(arg):
    """Accept an object, a reference, a pointer, or a raw address."""
    v = gdb.parse_and_eval(arg)
    code = v.type.strip_typedefs().code
    if code == gdb.TYPE_CODE_REF or code == gdb.TYPE_CODE_RVALUE_REF:
        v = v.referenced_value()
        code = v.type.strip_typedefs().code
    if code in (gdb.TYPE_CODE_PTR, gdb.TYPE_CODE_INT):
        return int(v)
    if v.address is None:
        raise gdb.GdbError("%s is not an lvalue; pass its address" % arg)
    return int(v.address)


def _vptr_of(obj):
    vptr = _qword(obj)
    s = _sym(vptr)
    if s is None or "vtable for" not in s[0]:
        raise gdb.GdbError(
            "the first word at 0x%x is 0x%x, which is not inside any vtable "
            "(not a polymorphic object, or not constructed yet?)" % (obj, vptr))
    return vptr, s


# -------------------------------------------------------------------------
# vtable
# -------------------------------------------------------------------------
def _dump_group(vptr, start, vname, obj=None):
    """Print one vtable group (header + slots); return the address after it.

    `start` bounds the backwards header walk; `obj` (if known) is the
    subobject whose vptr this is, used to resolve offset-to-top."""
    header = []
    i = -1
    while vptr + i * _PTR >= start:
        val = _qword(vptr + i * _PTR)
        if i == -1:
            label = "typeinfo       -> %s" % _symtext(val)
        elif i == -2:
            ott = _signed(val)
            label = "offset-to-top  = %d" % ott
            if ott and obj is not None:
                label += "   (complete object @ 0x%x)" % (obj + ott)
        else:
            if _is_code(val) or _is_typeinfo(val):
                break
            label = "vcall/vbase offset = %d" % _signed(val)
        header.append((i, val, label))
        i -= 1
    for i, val, label in reversed(header):
        print("  [%3d] 0x%016x  %s" % (i, val, label))

    # Slots: code pointers (or null slots followed by more code) that
    # still belong to the same vtable symbol.
    i, dtor_seen = 0, 0
    while True:
        addr = vptr + i * _PTR
        here = _sym(addr)
        if here is None or here[0] != vname:
            break
        val = _qword(addr)
        if val == 0:
            j = i + 1
            while _qword(vptr + j * _PTR) == 0 and j - i < 8:
                j += 1
            if not _is_code(_qword(vptr + j * _PTR)):
                break
            print("  [%3d] 0x%016x  (null)" % (i, 0))
            i += 1
            continue
        if not _is_code(val):
            break
        text = _symtext(val)
        note = ""
        if "::~" in text:
            note = "   <- complete dtor (D1)" if dtor_seen % 2 == 0 \
                else "   <- deleting dtor (D0)"
            dtor_seen += 1
        elif "__cxa_pure_virtual" in text:
            note = "   <- pure virtual"
        print("  [%3d] 0x%016x  %s%s" % (i, val, text, note))
        i += 1
    return vptr + i * _PTR


class VtableCmd(gdb.Command):
    """Dump a vtable, ABI header included.

Usage: vtable <object | pointer | reference | address>
       vtable <TypeName>        the whole "vtable for TypeName" symbol,
                                every group (also works for abstract classes)

  [-3..]  vcall / vbase offsets   (only with virtual inheritance)
  [-2]    offset-to-top           (this + offset == the complete object)
  [-1]    typeinfo pointer        (what typeid() and dynamic_cast use)
  [0..n]  virtual function slots  (the vptr points at slot 0)

Pass a pointer to a non-primary base (e.g. a Serializable* into a Sprite)
to see a secondary vtable: negative offset-to-top and thunk slots."""

    def __init__(self):
        super().__init__("vtable", gdb.COMMAND_DATA, gdb.COMPLETE_EXPRESSION)

    def invoke(self, arg, from_tty):
        if not arg:
            raise gdb.GdbError("usage: vtable <object|pointer|TypeName>")
        try:
            obj = _object_address(arg)
        except gdb.error:
            return self._by_type(arg)
        vptr, (vname, voff, _) = _vptr_of(obj)
        print("object @ 0x%x   vptr = 0x%x  <%s+%d>" % (obj, vptr, vname, voff))
        if vname.startswith("construction vtable"):
            print("  NOTE: a construction vtable -- this object is in the middle "
                  "of a base-class constructor/destructor")
        _dump_group(vptr, vptr - voff, vname, obj)

    def _by_type(self, name):
        try:
            start = int(gdb.parse_and_eval("(char *)&'vtable for %s'" % name))
        except gdb.error:
            raise gdb.GdbError("'%s' is neither an expression nor a class with "
                               "an emitted vtable" % name)
        vname = _sym(start)[0]
        pos, group = start, 0
        while True:
            ti = pos                     # the next typeinfo marks a group
            while _sym(ti) and _sym(ti)[0] == vname and \
                    not _is_typeinfo(_qword(ti)):
                ti += _PTR
            if not _sym(ti) or _sym(ti)[0] != vname:
                break
            vptr = ti + _PTR
            print("%s  group %d: vptr would be 0x%x <%s+%d>"
                  % ("" if group == 0 else "\n", group, vptr, vname, vptr - start))
            pos = _dump_group(vptr, pos, vname)
            group += 1


VtableCmd()


# -------------------------------------------------------------------------
# layout
# -------------------------------------------------------------------------
def _is_virtual_base(field):
    return field.is_base_class and getattr(field, "bitpos", None) is None


class LayoutCmd(gdb.Command):
    """Runtime byte map of an object, using its DYNAMIC type.

Usage: layout <object | pointer | reference>

Shows every base subobject (virtual bases at the place this particular
object put them), each vptr and the vtable it currently points into,
every data member, and the holes / padding left over."""

    def __init__(self):
        super().__init__("layout", gdb.COMMAND_DATA, gdb.COMPLETE_EXPRESSION)

    def invoke(self, arg, from_tty):
        if not arg:
            raise gdb.GdbError("usage: layout <object|pointer>")
        v = gdb.parse_and_eval(arg)
        if v.type.strip_typedefs().code in (gdb.TYPE_CODE_REF,
                                            gdb.TYPE_CODE_RVALUE_REF):
            v = v.referenced_value()
        if v.type.strip_typedefs().code != gdb.TYPE_CODE_PTR:
            if v.address is None:
                raise gdb.GdbError("%s is not an lvalue" % arg)
            v = v.address
        static_t = v.type.target()
        first = _sym(_qword(int(v))) if int(v) else None
        building = first is not None and first[0].startswith("construction vtable")
        if not building:                 # gdb's RTTI lookup chokes on these
            try:
                v = v.cast(v.dynamic_type)
            except gdb.error:
                pass
        obj = v.dereference()
        t = obj.type.strip_typedefs()
        if t.code != gdb.TYPE_CODE_STRUCT or t.sizeof == 0:
            raise gdb.GdbError("no complete type for %s (no debug info?) -- "
                               "try 'vtable %s' or 'rtti %s'" % (arg, arg, arg))
        top = int(obj.address)
        dyn = "" if str(t) == str(static_t.strip_typedefs()) else \
            "   (dynamic type; static type is %s)" % static_t
        print("%s @ 0x%x, sizeof = %d%s" % (t, top, t.sizeof, dyn))
        if building:
            print("  NOTE: under construction (construction vtable); showing "
                  "the static type")
        print("  offset  size")
        self.leaves = []
        deferred = []
        self._walk(obj, top, 1, deferred)
        seen = set()
        while deferred:
            vb = deferred.pop(0)
            key = str(vb.type.strip_typedefs())
            if key in seen:
                continue
            seen.add(key)
            print("  -- virtual base, placed by the most-derived class --")
            self._walk(vb, top, 1, deferred, virtual=True)
        self._holes(t.sizeof)

    def _row(self, off, size, depth, text):
        cols = "  %6d  %4d" % (off, size) if off is not None else " " * 14
        print("%s  %s%s" % (cols, "  " * depth, text))

    @staticmethod
    def _is_empty(t):
        return all(f.is_base_class and LayoutCmd._is_empty(f.type.strip_typedefs())
                   for f in t.fields()
                   if getattr(f, "bitpos", None) is not None or f.is_base_class)

    def _walk(self, val, top, depth, deferred, virtual=False):
        t = val.type.strip_typedefs()
        addr = int(val.address)
        tag = "  [virtual base]" if virtual else ""
        if self._is_empty(t):
            tag += "  [empty base: occupies no bytes]"
        self._row(addr - top, t.sizeof, depth - 1, "%s%s" % (t, tag))
        for f in t.fields():
            if f.is_base_class:
                try:
                    sub = val.cast(f.type)
                except gdb.error as e:
                    self._row(None, 0, depth,
                              "%s  <cannot locate: %s>" % (f.type, e))
                    continue
                if _is_virtual_base(f):
                    self._row(None, 0, depth,
                              "virtual %s  -> lives at offset %d (below)"
                              % (f.type, int(sub.address) - top))
                    deferred.append(sub)
                else:
                    self._walk(sub, top, depth + 1, deferred)
                continue
            if getattr(f, "bitpos", None) is None:
                continue                                   # static member
            off = addr - top + f.bitpos // 8
            size = f.type.sizeof
            if f.artificial and f.name and f.name.startswith("_vptr"):
                vptr = _qword(top + off)
                self._row(off, size, depth, "vptr  -> %s" % _symtext(vptr))
            else:
                self._row(off, size, depth, "%s %s" % (f.type, f.name))
            self.leaves.append((off, size))

    def _holes(self, total):
        end, holes = 0, []
        for off, size in sorted(self.leaves):
            if off > end:
                holes.append((end, off - end))
            end = max(end, off + size)
        if end < total:
            holes.append((end, total - end))
        if holes:
            print("  holes/padding: " + ", ".join(
                "[%d..%d) %d byte%s" % (o, o + n, n, "s" if n > 1 else "")
                for o, n in holes))
        else:
            print("  no holes")


LayoutCmd()


# -------------------------------------------------------------------------
# rtti / subclasses — decode the typeinfo objects themselves
#   __class_type_info       { vptr, name }                       no bases
#   __si_class_type_info    { vptr, name, base }                 single, public,
#                                                                non-virtual
#   __vmi_class_type_info   { vptr, name, u32 flags, u32 count,  everything else
#                             { base*, long offset_flags }[count] }
#       offset_flags: bit0 virtual, bit1 public, >>8 offset (for a virtual
#       base: where in the vtable its vbase offset lives, e.g. -24)
# -------------------------------------------------------------------------
def _typeinfo_name(ti):
    s = _sym(ti)
    if s and s[0].startswith("typeinfo for "):
        return s[0][len("typeinfo for "):]
    return "<typeinfo @ 0x%x>" % ti


def _typeinfo_bases(ti):
    kind = _symtext(_qword(ti))
    if "__si_class_type_info" in kind:
        return "single", 0, [(_qword(ti + 16), "public", 0, False)]
    if "__vmi_class_type_info" in kind:
        word = _qword(ti + 16)
        flags, count = word & 0xffffffff, word >> 32
        bases = []
        for k in range(count):
            base = _qword(ti + 24 + 16 * k)
            of = _signed(_qword(ti + 32 + 16 * k))
            bases.append((base, "public" if of & 2 else "non-public",
                          of >> 8, bool(of & 1)))
        return "vmi", flags, bases
    if "__class_type_info" in kind:
        return "none", 0, []
    return "not-a-class", 0, []


def _typeinfo_from_arg(arg):
    try:
        return _qword(_vptr_of(_object_address(arg))[0] - _PTR)
    except gdb.error:
        pass
    try:
        return int(gdb.parse_and_eval("&'typeinfo for %s'" % arg))
    except gdb.error:
        raise gdb.GdbError("no object or 'typeinfo for %s' symbol "
                           "(is the class polymorphic and used?)" % arg)


class RttiCmd(gdb.Command):
    """Print the inheritance tree as the compiler recorded it in RTTI.

Usage: rtti <object | pointer | TypeName>

This is exactly what dynamic_cast and catch-by-base walk at run time,
so it works without debug info (try: rtti ex, rtti ss)."""

    def __init__(self):
        super().__init__("rtti", gdb.COMMAND_DATA, gdb.COMPLETE_EXPRESSION)

    def invoke(self, arg, from_tty):
        if not arg:
            raise gdb.GdbError("usage: rtti <object|pointer|TypeName>")
        self._print(_typeinfo_from_arg(arg), "", "", True)

    def _print(self, ti, prefix, how, root):
        kind, flags, bases = _typeinfo_bases(ti)
        extra = ""
        if kind == "vmi" and flags:
            extra = "   [%s]" % ", ".join(n for b, n in ((1, "repeated base"),
                                                       (2, "diamond"))
                                          if flags & b)
        print("%s%s%s%s" % (prefix if root else "", _typeinfo_name(ti), how, extra))
        for k, (base, access, off, virt) in enumerate(bases):
            last = k == len(bases) - 1
            branch = prefix + ("`-- " if last else "|-- ")
            if virt:
                how = "   (%s virtual, vbase offset at vptr%+d)" % (access, off)
            else:
                how = "   (%s, at offset %d)" % (access, off)
            print(branch, end="")
            self._print(base, prefix + ("    " if last else "|   "), how, False)


RttiCmd()


class SubclassesCmd(gdb.Command):
    """List every class in the program whose RTTI says it derives from Type.

Usage: subclasses <TypeName>

Scans the typeinfo symbols (_ZTI*) of the executable, so only polymorphic
classes whose typeinfo was emitted show up."""

    def __init__(self):
        super().__init__("subclasses", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)

    def invoke(self, arg, from_tty):
        if not arg:
            raise gdb.GdbError("usage: subclasses <TypeName>")
        target = _typeinfo_from_arg(arg)
        exe = gdb.objfiles()[0].filename
        dump = gdb.execute("maint print msymbols -objfile %s" % exe,
                           to_string=True)
        tis = set()
        for line in dump.splitlines():
            m = re.search(r"\s(0x[0-9a-f]+|[0-9a-f]{8,})\s+_ZTI\S+", line)
            if m:
                tis.add(int(m.group(1), 16))
        found = []
        for ti in tis:
            if ti != target and self._derives(ti, target, 0):
                found.append(_typeinfo_name(ti))
        for name in sorted(found) or ["(none found)"]:
            print("  " + name)

    def _derives(self, ti, target, depth):
        if depth > 32:
            return False
        for base, _, _, _ in _typeinfo_bases(ti)[2]:
            if base == target or self._derives(base, target, depth + 1):
                return True
        return False


SubclassesCmd()
end
