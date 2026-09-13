# =========================================================================
# heap.gdb — helpers for a class on std::string storage and the glibc heap
#
#   gdb -q -x heap.gdb ./heap_tour
#
# Commands defined here:
#   strobj  <string>        raw libstdc++ std::string fields, SSO yes/no
#   chunk   <ptr>           decode the malloc header in front of a pointer
#   chunks  <ptr> <n>       walk n consecutive chunks by hand
#   heapwalk [n]            walk the whole [heap] segment (Python, safe)
#   bins                    tcache + unsorted bin state
#   unmangle <ptr-to-fd>    undo glibc's tcache/fastbin pointer obfuscation
#   maps                    the mappings that matter
#
# NOTE on convenience variables: never name one $cs, $ps, $sp, $pc, $fs ...
# On x86-64 those ARE registers, and "set $cs = 5" writes the CS segment
# register instead of creating a variable.  Hence the $mc_ / $so_ prefixes.
# =========================================================================

set confirm off
set pagination off
set print pretty on
set disable-randomization on

# -------------------------------------------------------------------------
# chunk <user-pointer>
#   glibc malloc puts a 16-byte header immediately before the pointer it
#   hands you (64-bit):
#       ptr-16  mchunk_prev_size   (size of the previous chunk, if free)
#       ptr-8   mchunk_size        (low 3 bits are flags)
#       ptr     your bytes start here
#   Flags: 0x1 PREV_INUSE, 0x2 IS_MMAPPED, 0x4 NON_MAIN_ARENA
# -------------------------------------------------------------------------
define chunk
  if $argc != 1
    printf "usage: chunk <user-pointer>\n"
  else
    set $mc_ptr   = (unsigned long *)($arg0)
    set $mc_prev  = *($mc_ptr - 2)
    set $mc_size  = *($mc_ptr - 1)
    set $mc_csize = $mc_size & ~7
    if $mc_csize == 0
      printf "** The size field here reads 0, so this is not a user pointer.\n"
      printf "** 'chunk' wants the pointer malloc returned, not the chunk header.\n"
      printf "** In heapwalk output: column 1 is the header, column 2 is the payload.\n"
      if (*($mc_ptr + 1) & ~7) != 0
        printf "** Looks like you meant:  chunk %p\n", (void *)($mc_ptr + 2)
      end
    else
    printf "chunk header @ %p   user data @ %p\n", (void *)($mc_ptr - 2), (void *)$mc_ptr
    printf "  prev_size  = 0x%lx\n", $mc_prev
    printf "  size field = 0x%lx   ->  chunk size = %lu bytes  (usable = %lu)\n", \
           $mc_size, $mc_csize, $mc_csize - 8
    printf "  flags      : PREV_INUSE=%d  IS_MMAPPED=%d  NON_MAIN_ARENA=%d\n", \
           ($mc_size & 1) != 0, ($mc_size & 2) != 0, ($mc_size & 4) != 0
    if ($mc_size & 2) != 0
      printf "  -> IS_MMAPPED: this block came straight from mmap(), not the arena\n"
    else
    if ($mc_ptr - 2) == (unsigned long *)main_arena.top
      printf "  -> this IS the top chunk: everything after it is unhanded-out space\n"
    else
      set $mc_next  = (unsigned long *)((char *)($mc_ptr - 2) + $mc_csize)
      set $mc_nsize = *($mc_next + 1)
      printf "  next chunk @ %p, size field 0x%lx\n", (void *)$mc_next, $mc_nsize
      if ($mc_nsize & 1) != 0
        printf "  -> next chunk's PREV_INUSE is SET   => THIS chunk is IN USE\n"
      else
        printf "  -> next chunk's PREV_INUSE is CLEAR => THIS chunk is FREE\n"
        printf "     (note: tcache/fastbin chunks keep this bit SET - see 'bins')\n"
      end
    end
    end
    printf "  raw qwords from header: "
    output/x *(unsigned long *)($mc_ptr - 2)@6
    printf "\n"
    end
  end
end
document chunk
Decode the glibc malloc chunk header sitting in front of a user pointer.
Usage: chunk <pointer returned by malloc/new>
end

# -------------------------------------------------------------------------
# chunks <user-pointer> <count>
#   Walks forward chunk by chunk.  Deliberately done with plain GDB
#   arithmetic so the class can see there is no magic: next = here + size.
# -------------------------------------------------------------------------
define chunks
  if $argc != 2
    printf "usage: chunks <user-pointer> <count>\n"
  else
    set $mw_hdr = (unsigned long *)((char *)($arg0) - 16)
    set $mw_i = 0
    printf "%-20s %8s %6s  %s\n", "chunk header", "size", "flags", "state"
    while $mw_i < $arg1
      set $mc_size  = *($mw_hdr + 1)
      set $mc_csize = $mc_size & ~7
      if $mc_csize == 0
        printf "%-20p        - size 0, stopping\n", (void *)$mw_hdr
        loop_break
      end
      if $mw_hdr == (unsigned long *)main_arena.top
        printf "%-20p %8lu %6s  TOP CHUNK (the wilderness) - stopping\n", \
               (void *)$mw_hdr, $mc_csize, "-"
        loop_break
      end
      set $mc_next = (unsigned long *)((char *)$mw_hdr + $mc_csize)
      if (*($mc_next + 1) & 1) != 0
        printf "%-20p %8lu %6lu  in use\n", (void *)$mw_hdr, $mc_csize, $mc_size & 7
      else
        printf "%-20p %8lu %6lu  FREE\n", (void *)$mw_hdr, $mc_csize, $mc_size & 7
      end
      set $mw_hdr = $mc_next
      set $mw_i = $mw_i + 1
    end
  end
end
document chunks
Walk N consecutive malloc chunks starting from a known user pointer.
Stops at the top chunk.  Usage: chunks <pointer> <count>
end

# -------------------------------------------------------------------------
# strobj <std::string lvalue>
#   libstdc++ (C++11 ABI) stores exactly three things:
#       _M_dataplus._M_p        pointer to the characters
#       _M_string_length        length
#       union { char _M_local_buf[16]; size_type _M_allocated_capacity; }
#   The whole small-string trick is: for short strings _M_p points back
#   into _M_local_buf, i.e. into the object itself.
# -------------------------------------------------------------------------
define strobj
  if $argc != 1
    printf "usage: strobj <a std::string variable>\n"
  else
    set $so_obj = (char *)&($arg0)
    set $so_ptr = ($arg0)._M_dataplus._M_p
    printf "object @ %p   sizeof = %d\n", (void *)$so_obj, sizeof($arg0)
    printf "  _M_p             = %p\n", (void *)$so_ptr
    printf "  _M_string_length = %lu\n", ($arg0)._M_string_length
    if (char *)$so_ptr >= $so_obj && (char *)$so_ptr < $so_obj + sizeof($arg0)
      printf "  characters       = "
      if ($arg0)._M_string_length > 0
        output *$so_ptr@($arg0)._M_string_length
      else
        printf "(empty)"
      end
      printf "\n"
      printf "  -> _M_p points %d bytes INTO the object itself.\n", \
             (int)((char *)$so_ptr - $so_obj)
      printf "     Small String Optimisation: zero heap allocations.\n"
    else
      printf "  _M_allocated_capacity = %lu\n", ($arg0)._M_allocated_capacity
      printf "  -> _M_p points outside the object; header below.\n"
      chunk $so_ptr
    end
  end
end
document strobj
Dump the raw libstdc++ std::string fields and say whether SSO is in effect.
Usage: strobj <std::string variable>
end

# -------------------------------------------------------------------------
# maps
# -------------------------------------------------------------------------
define maps
  pipe info proc mappings | grep -E "Start Addr|heap|stack|rw-p *$"
end
document maps
Show process mappings, keeping the [heap], [stack] and anonymous rw- regions.
end

# =========================================================================
# Python helpers: safe whole-heap walk and bin inspection.
# GDB on every mainstream distro is built with Python, so this needs no
# extra install.  main_arena/tcache typing needs glibc debug info:
#   Debian/Ubuntu: apt install libc6-dbg
#   Fedora/RHEL:   dnf debuginfo-install glibc
# =========================================================================
python
import gdb

PREV_INUSE, IS_MMAPPED, NON_MAIN_ARENA = 1, 2, 4


def _qword(addr):
    inf = gdb.selected_inferior()
    return int.from_bytes(bytes(inf.read_memory(addr, 8)), "little")


def _heap_bounds():
    pid = gdb.selected_inferior().pid
    with open("/proc/%d/maps" % pid) as fh:
        for line in fh:
            if line.rstrip().endswith("[heap]"):
                lo, hi = line.split()[0].split("-")
                return int(lo, 16), int(hi, 16)
    return None, None


def _top():
    try:
        return int(gdb.parse_and_eval("main_arena.top"))
    except gdb.error:
        return None


def _unmangle(field_addr, stored):
    """glibc >= 2.32 stores next pointers XORed with (address >> 12)."""
    return (field_addr >> 12) ^ stored


def _free_set():
    """Map payload address -> which free list it is parked in.

    This matters because glibc does NOT clear the next chunk's PREV_INUSE
    bit for tcache/fastbin chunks, so header bits alone say "in use"."""
    out = {}
    try:
        tc = gdb.parse_and_eval("*tcache")
        for i in range(64):
            if not int(tc["counts"][i]):
                continue
            node = int(tc["entries"][i])
            seen = 0
            while node and seen < 64:
                out[node] = "tcache[%d]" % i
                node = _unmangle(node, _qword(node))
                seen += 1
    except gdb.error:
        pass
    try:
        arena = gdb.parse_and_eval("main_arena")
        fast = arena["fastbinsY"]
        for i in range(int(fast.type.range()[1]) + 1):
            chunk = int(fast[i])
            seen = 0
            while chunk and seen < 64:
                out[chunk + 16] = "fastbin[%d]" % i
                chunk = _unmangle(chunk + 16, _qword(chunk + 16))
                seen += 1
    except gdb.error:
        pass
    return out


def _flagstr(size):
    return "".join([
        "P" if size & PREV_INUSE else "-",
        "M" if size & IS_MMAPPED else "-",
        "N" if size & NON_MAIN_ARENA else "-",
    ])


class HeapWalk(gdb.Command):
    """heapwalk [limit] -- walk every chunk in the main [heap] segment.

    Shows each chunk's header address, the payload address your code sees,
    the rounded chunk size, the three flag bits, and whether the chunk is
    free (read off the *next* chunk's PREV_INUSE bit)."""

    def __init__(self):
        super(HeapWalk, self).__init__("heapwalk", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        lo, hi = _heap_bounds()
        if lo is None:
            print("no [heap] segment yet - nothing has been taken from brk()")
            return
        limit = int(arg) if arg.strip() else 1000
        top = _top()
        print("[heap] 0x%x - 0x%x  (%d KiB)" % (lo, hi, (hi - lo) // 1024))
        print("%-18s %-18s %8s %5s  %s" %
              ("chunk", "payload", "size", "flags", "state"))
        parked = _free_set()
        cur, n, used, freed = lo, 0, 0, 0
        while cur + 16 <= hi and n < limit:
            size = _qword(cur + 8)
            csize = size & ~7
            if csize == 0:
                print("%-18s size 0 - stopping" % hex(cur))
                break
            if top is not None and cur == top:
                print("%-18s %-18s %8d %5s  TOP CHUNK (wilderness)" %
                      (hex(cur), hex(cur + 16), csize, _flagstr(size)))
                break
            nxt = cur + csize
            if nxt + 16 > hi:
                print("%-18s %-18s %8d %5s  runs to end of segment" %
                      (hex(cur), hex(cur + 16), csize, _flagstr(size)))
                break
            if cur + 16 in parked:
                state = "FREE  <- %s" % parked[cur + 16]
            elif _qword(nxt + 8) & PREV_INUSE:
                state = "in use"
            else:
                state = "FREE  <- unsorted/small/large bin"
            if state == "in use":
                used += csize
            else:
                freed += csize
            print("%-18s %-18s %8d %5s  %s" %
                  (hex(cur), hex(cur + 16), csize, _flagstr(size), state))
            cur, n = nxt, n + 1
        print("-- %d chunks: %d bytes in use, %d bytes free (excl. top)" %
              (n, used, freed))


class Bins(gdb.Command):
    """bins -- show the tcache bins and the unsorted bin."""

    def __init__(self):
        super(Bins, self).__init__("bins", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        try:
            tc = gdb.parse_and_eval("*tcache")
        except gdb.error as exc:
            print("tcache unavailable (%s); install glibc debug info" % exc)
            tc = None
        if tc is not None:
            print("tcache (per-thread, lock-free, 7 chunks max per bin):")
            empty = True
            for i in range(64):
                cnt = int(tc["counts"][i])
                if cnt:
                    empty = False
                    print("  bin[%2d] chunk size %4d : %d entry(ies), head -> %s"
                          % (i, 32 + 16 * i, cnt, hex(int(tc["entries"][i]))))
            if empty:
                print("  (all empty)")
        try:
            arena = gdb.parse_and_eval("main_arena")
        except gdb.error:
            print("main_arena unavailable; install glibc debug info")
            return
        unsorted_head = int(arena["bins"][0])
        bin_addr = int(arena["bins"].address) - 16
        print("unsorted bin:")
        if unsorted_head == bin_addr:
            print("  empty")
        else:
            size = _qword(unsorted_head + 8) & ~7
            print("  first chunk @ %s, size %d" % (hex(unsorted_head), size))
        print("top chunk @ %s" % hex(int(arena["top"])))


class Unmangle(gdb.Command):
    """unmangle <address-of-fd-field> -- undo glibc >= 2.32 pointer obfuscation.

    A freed tcache/fastbin chunk stores its next pointer as
        stored = (address_of_that_field >> 12) XOR real_pointer
    so a leaked heap pointer alone is not enough to forge a free list."""

    def __init__(self):
        super(Unmangle, self).__init__("unmangle", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        if not arg.strip():
            print("usage: unmangle <address of the fd field (= payload addr)>")
            return
        addr = int(gdb.parse_and_eval(arg))
        stored = _qword(addr)
        real = (addr >> 12) ^ stored
        tail = "   (NULL - end of this bin's list)" if real == 0 else ""
        print("field @ %s" % hex(addr))
        print("  stored value   : %s" % hex(stored))
        print("  key (addr >> 12): %s" % hex(addr >> 12))
        print("  real pointer   : %s%s" % (hex(real), tail))


HeapWalk()
Bins()
Unmangle()
print("heap.gdb loaded: strobj / chunk / chunks / heapwalk / bins / unmangle / maps")
end


