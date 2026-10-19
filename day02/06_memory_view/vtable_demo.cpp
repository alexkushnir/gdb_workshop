// vtble_demo.cpp — how GCC lays out polymorphic objects, and how to see it in GDB.
//
//   g++ -std=c++20 -g -O0 -fno-omit-frame-pointer vtble_demo.cpp -o vtble_demo
//   gdb -q -x vtable.gdb ./vtble_demo
//
// Breakpoint anchor for GDB:  b stage      (then "up" to reach the locals)
//
// Itanium C++ ABI in one screen (GCC and Clang on Linux, x86-64):
//
//   object                         vtable ("vtable for T", symbol _ZTV1T)
//   +--------+                     +---------------------+
//   | vptr   |------+              | [-3] vbase offset   |  only with virtual bases
//   | data.. |      |              | [-2] offset-to-top  |  0 for the primary vptr
//   +--------+      |              | [-1] &typeinfo      |  "typeinfo for T" (_ZTI1T)
//                   +------------->| [0]  virtual fn #0  |  vptr points HERE, not
//                                  | [1]  virtual fn #1  |  at the start of the symbol
//                                  +---------------------+
//
//   - A virtual destructor takes TWO slots: "complete" D1 and "deleting" D0.
//   - Multiple inheritance = one vptr per polymorphic base, each pointing
//     into a secondary vtable whose slots are "non-virtual thunks" that fix
//     up `this` before jumping to the real override.
//   - Virtual inheritance adds vbase offsets, and during construction the
//     object temporarily uses "construction vtables" (found through the VTT).
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

// Every interesting moment funnels through here, so one breakpoint
// ("b stage") walks the whole lesson.  extern "C" keeps the name clean.
extern "C" void stage(const char* what) {
    std::printf("\n========== stage: %s ==========\n", what);
    std::fflush(stdout);
}

// ---------------------------------------------------------------------------
// Act I — what "virtual" costs: the vptr
// ---------------------------------------------------------------------------
struct PlainWidget {      // no virtual functions: sizeof == 56
    int         id;       // 4 bytes  + 4 padding
    double      weight;   // 8 bytes
    std::string name;     // 32 bytes (libstdc++ C++11 ABI)
    char        tag;      // 1 byte   + 7 padding
};

struct Widget {           // same members + a vptr at offset 0: sizeof == 64
    Widget(int id, double weight, std::string name, char tag)
        : id(id), weight(weight), name(std::move(name)), tag(tag) {}
    virtual ~Widget() = default;                       // slots 0 and 1 (D1, D0)
    virtual void foo() const { std::cout << "I'm basic!\n"; }  // slot 2

    int         id;
    double      weight;
    std::string name;
    char        tag;
};

struct WiWi : Widget {
    using Widget::Widget;
    ~WiWi() override = default;
    void foo() const override { std::cout << "I'm more advanced!\n"; }
    virtual void bar() const { std::cout << "Only WiWi has bar()\n"; }  // slot 3

    // Widget is not a POD, so the Itanium ABI lets the derived class reuse
    // Widget's tail padding: `level` lands at offset 60, sizeof(WiWi) == 64.
    int level = 42;
};

// ---------------------------------------------------------------------------
// Act III — multiple inheritance: two vptrs, thunks, pointer adjustment
// ---------------------------------------------------------------------------
struct Drawable {
    virtual ~Drawable() = default;
    virtual void draw() const = 0;   // pure: slot holds __cxa_pure_virtual
    int z_order = 1;
};

struct Serializable {
    virtual ~Serializable() = default;
    virtual void save() const { std::cout << "Serializable::save\n"; }
    int version = 2;
};

struct Sprite : Drawable, Serializable {
    void draw() const override { std::cout << "Sprite::draw\n"; }
    void save() const override { std::cout << "Sprite::save\n"; }
    int frame = 3;
};

// ---------------------------------------------------------------------------
// Act IV/V — virtual inheritance (the std::iostream shape) and construction
// ---------------------------------------------------------------------------
struct Stream {
    Stream() {
        kind();                          // dispatches to Stream::kind, always
        stage("ctor: Stream   (vptr -> vtable for Stream)");
    }
    virtual ~Stream() = default;
    virtual const char* kind() const { return "Stream"; }
    int fd = 0;
};

struct InStream : virtual Stream {
    InStream() { stage("ctor: InStream (vptr -> construction vtable)"); }
    const char* kind() const override { return "InStream"; }
    virtual void read() {}
    int rpos = 10;
};

struct OutStream : virtual Stream {
    const char* kind() const override { return "OutStream"; }
    virtual void write() {}
    int wpos = 20;
};

struct IOStream : InStream, OutStream {
    IOStream() { stage("ctor: IOStream (vptr -> final vtable)"); }
    const char* kind() const override { return "IOStream"; }
    int mode = 30;
};

int main() {
    // ----- Act I — the vptr and what it costs ------------------------------
    //   p sizeof(PlainWidget)       p sizeof(Widget)           56 vs 64
    //   ptype /o PlainWidget        ptype /o Widget
    //        -> Widget's members start at offset 8: ptype /o does NOT print
    //           the vptr, the gap at [0..8) is it
    //   ptype /o WiWi               -> `level` at 60: tail padding reused
    //   p w1                        -> _vptr.Widget = <vtable for Widget+16>
    //   info vtbl w1                info vtbl w2         (slot 3 = bar)
    //   p &Widget::foo              -> "&virtual table offset 2"
    //   info symbol *(void**)&w2    -> vtable for WiWi + 16 in .data.rel.ro
    //   x/6ag *(void***)&w2 - 2     -> raw: offset-to-top, typeinfo, slots
    //   vtable w2                   layout w2       (helpers, vtable.gdb)
    PlainWidget plain{1, 0.5, "plain", 'P'};
    Widget      w1{7, 1.5, "a name long enough to force a heap allocation", 'X'};
    WiWi        w2{4, 2.2, "I'm ok!", 'Y'};
    stage("Act I   vptr and layout          (ptype /o, p w1, info vtbl)");

    // ----- Act II — static vs dynamic type ----------------------------------
    //   p *p                        -> only the Widget part
    //   set print object on
    //   p *p                        -> (WiWi) {..., level = 42}
    //   ptype p                     -> /* real type = WiWi * */
    //   p typeid(*p)                p typeid(*p).__name     -> "4WiWi"
    //   p dynamic_cast<WiWi*>(p)    p r
    //   p p->foo()                  -> virtual call made by gdb itself
    //   layout p                    rtti p
    //   set print object off
    Widget* p = &w2;                 // static type Widget*, dynamic type WiWi
    Widget& r = w2;
    p->foo();
    stage("Act II  static vs dynamic type   (set print object on, typeid)");

    // ----- Act III — multiple inheritance -----------------------------------
    //   p &sprite   p as_drawable   p as_serializable     -> +16 adjustment
    //   p (char*)as_serializable - (char*)&sprite        -> 16
    //   ptype /o Sprite             p sprite              -> two _vptr fields
    //   info vtbl sprite            -> 2 tables; "non-virtual thunk to ..."
    //   vtable as_serializable      -> offset-to-top = -16, thunk slots
    //   vtable Sprite               -> the whole symbol, both groups
    //   vtable Drawable             -> abstract: __cxa_pure_virtual slot
    //   info functions -n ::save    rbreak ::save     (every override)
    //   disassemble 'non-virtual thunk to Sprite::save() const'
    //                               -> sub $0x10,%rdi ; jmp Sprite::save
    //   subclasses Drawable         rtti sprite
    Sprite        sprite;
    Drawable*     as_drawable     = &sprite;  // same address as &sprite
    Serializable* as_serializable = &sprite;  // &sprite + 16  (this-adjust)
    as_serializable->save();                  // goes through a thunk
    stage("Act III multiple inheritance     (two vptrs, thunks)");

    // ----- Act IV — an object under construction (3 stops) ------------------
    // At each stop:  up ;  vtable this ;  p this->kind()
    //   in Stream()   -> vtable for Stream, kind() == "Stream"
    //   in InStream() -> construction vtable for InStream-in-IOStream
    //                    (p *this / info vtbl this now warn: gdb only knows
    //                    "vtable for T" symbols; layout this still works)
    //   in IOStream() -> vtable for IOStream, kind() == "IOStream"
    //   bt                          -> note __vtt_parm passed to InStream()
    //   x/7ag &'VTT for IOStream'   -> the table of vptrs the ctors install
    //
    // ----- Act V — virtual inheritance: the std::iostream shape -------------
    //   ptype /o IOStream           -> claims "16-byte padding": that is
    //                                  where the virtual Stream really is
    //   layout io                   -> Stream placed at 32, shared by both
    //   p io                        info vtbl io  (misses IOStream's dtor
    //                                              slots; compare vtable io)
    //   p (char*)as_stream - (char*)&io           -> 32, read at run time
    //   vtable io                   -> [-3] vbase offset = 32
    //   vtable as_stream            -> "virtual thunk to IOStream::kind()"
    //   rtti io                     -> [diamond], vbase offset at vptr-24
    IOStream io;
    Stream*  as_stream = &io;                 // virtual base: offset read at run time
    stage("Act V   virtual inheritance      (vbase offset, layout)");

    // ----- Act VI — libstdc++ uses all of this ------------------------------
    //   set print object on
    //   p sp._M_refcount._M_pi      -> _Sp_counted_ptr_inplace<WiWi, ...>
    //   info vtbl sp._M_refcount._M_pi    (_M_dispose, _M_destroy, ...)
    //   layout sp._M_refcount._M_pi -> control block + object in one alloc
    //   p os    p ex                -> full objects only if libstdc++'s debug
    //                                  info is installed (the workshop image
    //                                  has it); without it: <incomplete type>
    //   info sharedlibrary          -> "(*)" marks libraries without it
    //   info symbol *(void**)&os    -> ELF symbols name the type either way
    //   vtable ex      rtti ex      rtti ss   (no debug info needed)
    //   ptype /o std::runtime_error -> _M_msg is a std::__cow_string
    //   info vtbl ex                -> what() slot reads logic_error::what:
    //                                  the two share one address in libstdc++
    std::shared_ptr<Widget> sp = std::make_shared<WiWi>(9, 9.9, "shared", 'S');
    std::stringstream       ss;
    std::ostream&           os = ss;
    std::runtime_error      err{"boom"};
    const std::exception&   ex = err;
    os << "hello";
    stage("Act VI  libstdc++ uses it too    (shared_ptr, iostreams, exceptions)");

    (void)plain; (void)r; (void)as_drawable; (void)as_stream; (void)ex;
    return 0;
}
