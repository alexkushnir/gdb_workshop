// heap_tour.cpp — a guided tour of std::string storage and the glibc heap.
//
//   g++ -std=c++20 -g -O0 -fno-omit-frame-pointer heap_tour.cpp -o heap_tour
//
// Breakpoint anchor for GDB:  b stage
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <string>

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

    stage("strings alive");   // <-- inspect all six strings here

    // ---------------------------------------------------------------
    // Act II — an object with a string member
    // ---------------------------------------------------------------
    Widget w{7, 1.5, "a name long enough to force a heap allocation", 'X'};
    std::printf("&w=%p  &w.name=%p  w.name.data()=%p\n",
                (void*)&w, (void*)&w.name, (void*)w.name.data());
    std::fflush(stdout);

    stage("widget");          // <-- ptype/o Widget, p w, p/r w.name

    // ---------------------------------------------------------------
    // Act III — raw chunks, so we can watch them change state
    // ---------------------------------------------------------------
    void* a     = std::malloc(24);    // -> 32-byte chunk
    void* b     = std::malloc(40);    // -> 48-byte chunk
    void* c     = std::malloc(200);   // -> 208-byte chunk, b's neighbour
    void* d     = std::malloc(2000);  // -> 2016-byte chunk, too big for tcache
    void* guard = std::malloc(64);    // keeps d away from the top chunk
    std::memset(a, 0xAA, 24);
    std::memset(b, 0xBB, 40);
    std::memset(c, 0xCC, 200);
    std::memset(d, 0xDD, 2000);
    std::printf("a=%p b=%p c=%p d=%p guard=%p\n", a, b, c, d, guard);
    std::fflush(stdout);

    stage("five raw chunks in use");

    // Small free -> tcache.  Note glibc deliberately does NOT clear the
    // next chunk's PREV_INUSE bit for tcache/fastbin chunks.
    std::free(a);
    stage("a freed -> tcache");

    // tcache is LIFO, so the very next same-size request gets it back.
    void* a2 = std::malloc(24);
    std::printf("malloc(24) again -> %p  (same as a? %s)\n",
                a2, a2 == a ? "YES" : "no");
    std::fflush(stdout);
    stage("a recycled from tcache");

    // Two adjacent small frees: both land in tcache and are NOT merged.
    std::free(b);
    std::free(c);
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
    stage("e and f allocated (1504-byte chunks)");

    std::free(e);
    std::free(f);             // adjacent + both above the tcache limit
    stage("e and f freed -> consolidated into one big free chunk");

    std::free(d);
    stage("d freed -> unsorted bin");

    // ---------------------------------------------------------------
    // Act V — giving the mmap'd region back to the kernel
    // ---------------------------------------------------------------
    huge.clear();
    huge.shrink_to_fit();
    stage("mmap region released");

    malloc_stats();
    return 0;
}

