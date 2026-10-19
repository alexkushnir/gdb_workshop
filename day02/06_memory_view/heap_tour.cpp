// heap_tour.cpp — a guided tour of std::string storage and the glibc heap.
//
//   g++ -std=c++20 -g -O0 -fno-omit-frame-pointer heap_tour.cpp -o heap_tour
//   gdb -q -x heap.gdb ./heap_tour
//   (gdb) b stage
//   (gdb) run
//   (gdb) up              <- at every stop: go to main's frame, then the
//                            commands listed above that stage() call
//   (gdb) c               <- next stage
//
// glibc malloc (64-bit) in one screen:
//
//   chunk header    +--------------------+
//                   | prev_size          |  only meaningful if the previous
//                   | size | A | M | P   |  chunk is free; low 3 bits = flags
//   malloc() ---->  +--------------------+
//   returns this    | your bytes ...     |  when free: fd/bk, or the mangled
//                   |                    |  tcache "next" + the tcache key
//                   +--------------------+
//
//   chunk size = max(32, round_up(request + 8, 16))
//       malloc(24) -> 32    malloc(40) -> 48    malloc(1500) -> 1520
//   Where a freed chunk goes:
//       chunk <= 1040 bytes (request <= 1032): tcache, 7 per size, LIFO, never merged
//       bigger: merged with free neighbours, parked in the unsorted bin, later
//               sorted into small (< 1024) / large bins
//       request >= mp_.mmap_threshold (128 KiB at start): its own mmap()
//
// Useful at any point:
//   b __libc_malloc if bytes == 200      who allocates this size?  (then: bt)
//   p mp_                                malloc's tunables and statistics
//   p *tcache                            this thread's tcache, raw
//   p mallinfo2()                        what malloc_stats() summarises
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <string>
#include <vector>

struct Widget {
    int         id;      // 4 bytes  + 4 padding
    double      weight;  // 8 bytes
    std::string name;    // 32 bytes (libstdc++ C++11 ABI)
    char        tag;     // 1 byte   + 7 padding
};

// Every interesting moment funnels through here, so one breakpoint
// ("b stage") walks the whole lesson.  extern "C" keeps the name clean.
extern "C" void stage(const char* what) {
    std::printf("\n========== stage: %s ==========\n", what);
    std::fflush(stdout);
}

static void describe(const char* label, const std::string& s) {
    const char* obj  = reinterpret_cast<const char*>(&s);
    const char* data = s.data();
    bool inside = (data >= obj) && (data < obj + sizeof(std::string));
    std::printf("%-7s len=%-8zu cap=%-8zu object@%p data@%p  %s\n",
                label, s.size(), s.capacity(),
                static_cast<const void*>(obj), static_cast<const void*>(data),
                inside ? "INSIDE the object  -> no allocation (SSO)"
                       : "outside the object -> allocated");
    std::fflush(stdout);
}

int main() {
    // ---------------------------------------------------------------
    // Act I — where does a std::string keep its characters?
    // ---------------------------------------------------------------
    std::string sso    = "short";                 // 5   chars -> SSO buffer
    std::string edge15 = std::string(15, 'e');    // 15  chars -> last SSO size
    std::string edge16 = std::string(16, 'E');    // 16  chars -> first heap size
    std::string medium = std::string(40, 'm');    // 40  chars -> small heap chunk
    std::string big    = std::string(1000, 'b');  // 1 KB      -> bigger heap chunk
    std::string huge   = std::string(1u << 20, 'h'); // 1 MiB  -> served by mmap

    describe("sso",    sso);
    describe("edge15", edge15);
    describe("edge16", edge16);
    describe("medium", medium);
    describe("big",    big);
    describe("huge",   huge);

    std::printf("\nsizeof(std::string) = %zu\n", sizeof(std::string));
    std::printf("malloc_usable_size(medium.data()) = %zu\n",
                malloc_usable_size(const_cast<char*>(medium.data())));
    std::printf("malloc_usable_size(huge.data())   = %zu\n",
                malloc_usable_size(const_cast<char*>(huge.data())));

    // GDB:
    //   p sizeof(std::string)       -> 32
    //   p sso                       p/r sso       (pretty printer vs raw)
    //   strobj sso                  -> _M_p points 16 bytes INTO the object
    //   strobj edge15               strobj edge16 -> 15 is SSO, 16 is heap
    //   strobj medium               -> 64-byte chunk: usable 56 (cap 40 + NUL fits)
    //   strobj huge                 -> IS_MMAPPED: not in [heap] at all
    //   maps                        -> huge's 1 MiB anonymous mapping
    //   heapwalk                    -> chunks labelled with the locals that
    //                                  point into them; three were there
    //                                  before main's first allocation:
    //       656   tcache_perthread_struct          (p *tcache)
    //       73744 libstdc++'s emergency exception pool, so `throw` works
    //             even when malloc fails           (p/x (long)
    //             '(anonymous namespace)::emergency_pool'.arena
    //             -- needs libstdc++ debug info)
    //       4112  stdout's buffer, made by the first printf
    //                                              (p stdout->_IO_buf_base)
    //   p mp_.mmap_threshold        -> 131072: why huge got its own mmap
    stage("strings alive");

    // ---------------------------------------------------------------
    // Act II — an object with a string member
    // ---------------------------------------------------------------
    Widget w{7, 1.5, "a name long enough to force a heap allocation", 'X'};
    std::printf("&w=%p  &w.name=%p  w.name.data()=%p\n",
                (void*)&w, (void*)&w.name, (void*)w.name.data());
    std::fflush(stdout);

    // GDB:
    //   ptype /o Widget             -> 4-byte hole after id, 7 bytes of tail
    //                                  padding; name is 32 bytes at offset 16
    //   p w                         p/r w.name
    //   p &w.name                   p w.name._M_dataplus._M_p
    //   strobj w.name               -> the object is on the stack, its
    //                                  characters are a 64-byte heap chunk
    //   heapwalk                    -> the new chunk is labelled w.name.data()
    stage("widget");

    // ---------------------------------------------------------------
    // Act III — raw chunks, so we can watch them change state
    // ---------------------------------------------------------------
    void* a     = std::malloc(24);    // -> 32-byte chunk
    void* b     = std::malloc(40);    // -> 48-byte chunk
    void* c     = std::malloc(200);   // -> 208-byte chunk, b's neighbour
    void* d     = std::malloc(2000);  // -> 2016-byte chunk, too big for tcache
    void* guard = std::malloc(64);    // keeps d away from the top chunk
    std::memset(a, 0xAAAAAA, 24);
    std::memset(b, 0xBB, 40);
    std::memset(c, 0xCC, 200);
    std::memset(d, 0xDD, 2000);
    std::printf("a=%p b=%p c=%p d=%p guard=%p\n", a, b, c, d, guard);
    std::fflush(stdout);

    // GDB:
    //   chunk a                     -> size field 0x21 = 32 | PREV_INUSE
    //   x/6gx (char*)a - 16         -> the same header, by hand
    //   chunks a 6                  -> walk a, b, c, d, guard, top
    //   p (char*)b - (char*)a       -> 32: one chunk apart, no gaps
    //   heapwalk                    bins          (everything in use)
    stage("five raw chunks in use");

    // Small free -> tcache.  Note glibc deliberately does NOT clear the
    // next chunk's PREV_INUSE bit for tcache/fastbin chunks.
    std::free(a);
    // GDB:
    //   chunk a                     -> still "IN USE": the bit lies for tcache
    //   x/2gx a                     -> [0] mangled next pointer, [1] tcache key
    //   unmangle a                  -> real next = NULL: a is alone in its bin
    //   p/x tcache_key              -> equals [1]; free() checks it to report
    //                                  "free(): double free detected in tcache 2"
    //   p tcache->counts[0]         p tcache->entries[0]
    //   bins                        heapwalk      (a is now a dangling pointer)
    stage("a freed -> tcache");

    // tcache is LIFO, so the very next same-size request gets it back.
    void* a2 = std::malloc(24);
    std::printf("malloc(24) again -> %p  (same as a? %s)\n",
                a2, a2 == a ? "YES" : "no");
    std::fflush(stdout);
    // GDB:
    //   p a2 == a                   -> true
    //   bins                        -> tcache bin for 32 is empty again
    //   x/2gx a2                    -> the key was wiped on reuse; the
    //                                  stale next pointer is still there
    stage("a recycled from tcache");

    // Two adjacent small frees: both land in tcache and are NOT merged.
    std::free(b);
    std::free(c);
    // GDB:
    //   heapwalk                    -> two FREE chunks side by side, 48 + 208
    //   bins                        -> one tcache bin per size
    //   chunk b                     -> c's PREV_INUSE still set: no merge
    stage("b and c freed -> tcache, adjacent but NOT merged");

    // ---------------------------------------------------------------
    // Act IV — chunks too big for tcache DO consolidate
    // ---------------------------------------------------------------
    void* e      = std::malloc(1500);
    void* f      = std::malloc(1500);
    void* guard2 = std::malloc(64);
    std::memset(e, 0xEE, 1500);
    std::memset(f, 0xFF, 1500);
    std::printf("e=%p f=%p guard2=%p\n", e, f, guard2);
    std::fflush(stdout);
    // GDB:
    //   chunk e                     -> 1520, not 1500: request + 8, rounded to 16
    //   p mp_.tcache_max_bytes      -> 1032: e and f are too big for tcache
    //   heapwalk                    -> e, f, guard2 carved off the top chunk
    stage("e and f allocated (1520-byte chunks)");

    std::free(e);
    std::free(f);             // adjacent + both above the tcache limit
    // GDB:
    //   heapwalk                    -> one 3040-byte FREE chunk where e and f were
    //   chunk guard2                -> PREV_INUSE now CLEAR, prev_size = 3040
    //   bins                        -> the merged chunk is in the unsorted bin
    stage("e and f freed -> consolidated into one big free chunk");

    std::free(d);             // neighbours: c (in tcache, looks in use) and guard
    // GDB:
    //   bins                        -> unsorted bin now holds d AND e+f
    //   heapwalk                    -> d stays 2016: c "looks" in use, so no merge
    //   chunk guard                 -> PREV_INUSE clear, prev_size = 2016
    stage("d freed -> unsorted bin, not merged with anything");

    // ---------------------------------------------------------------
    // Act V — mmap'd chunks, and a threshold that moves
    // ---------------------------------------------------------------
    huge.clear();
    huge.shrink_to_fit();
    // GDB:
    //   maps                        -> the 1 MiB mapping is gone (munmap)
    //   strobj huge                 -> back to SSO: _M_p points inside again
    //   p mp_.mmap_threshold        -> 1052672, was 131072: freeing an mmap'd
    //                                  chunk raises the threshold to its size
    //   p mp_.trim_threshold        -> raised to twice that
    stage("mmap region released");

    std::string again(1u << 20, 'H');
    // GDB:
    //   strobj again                -> same 1 MiB request, NOT mmapped now
    //   maps                        -> [heap] grew by ~1 MiB instead
    //   heapwalk                    -> again.data() is a 1 MiB chunk in [heap]
    //   bins                        -> d and e+f left the unsorted bin: while
    //                                  searching for 1 MiB, malloc sorted them
    //                                  into large bins
    stage("1 MiB again -> now from [heap], the threshold moved");

    // ---------------------------------------------------------------
    // Act VI — C++ on top of malloc
    // ---------------------------------------------------------------
    Widget* nw  = new Widget{8, 2.5, "heap widget, heap name as well", 'N'};
    Widget* arr = new Widget[3]{};
    std::vector<int> v;
    for (int i = 0; i < 17; ++i) {
        const int* before = v.data();
        v.push_back(i);
        if (v.data() != before)
            std::printf("v grew to capacity %2zu: buffer %p\n",
                        v.capacity(), static_cast<const void*>(v.data()));
    }
    std::fflush(stdout);
    // GDB:
    //   p sizeof(Widget)            -> 56 (no vptr in this Widget)
    //   chunk nw                    -> new Widget is an ordinary 64-byte chunk
    //   heapwalk                    -> "d, nw": nw was carved out of d's old
    //                                  chunk, so the dangling d now aliases a
    //                                  live object (use-after-free in waiting)
    //   strobj nw->name             -> its characters reuse b's 48-byte chunk,
    //                                  straight from tcache
    //   heapwalk                    -> "arr (+8)": new[] of a type with a
    //                                  destructor puts the element count in
    //                                  front of the array
    //   x/gx (char*)arr - 8         -> 3: the array cookie delete[] reads
    //   p *arr@3                    -> the three elements after it
    //   chunk ((char*)arr-8)        -> 192 = round_up(8 + 8 + 3 * 56, 16)
    //                                  (no spaces: user commands split args)
    //   p v                         -> capacity 32 after 17 push_backs
    //   heapwalk                    -> v's old buffers (32, 32, 48, 80) sit
    //                                  FREE in tcache; v.reserve(17) avoids it
    //   b 'operator new(unsigned long)'   then  run   (restarts the program)
    //                               -> first hit: sz=17, edge16's buffer;
    //                                  bt shows std::allocator<char> calling it
    //   finish                      -> the pointer operator new returned
    stage("new, new[] and a growing vector");

    delete nw;
    delete[] arr;
    malloc_stats();
    return 0;
}
