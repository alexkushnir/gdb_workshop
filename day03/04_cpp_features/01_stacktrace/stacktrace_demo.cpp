// stacktrace_demo.cpp - C++23 <stacktrace>
//
//   g++ -std=c++23 -g -O0 stacktrace_demo.cpp -o stacktrace_demo -lstdc++exp
//
// Three build requirements, all easy to get wrong:
//   -lstdc++exp   GCC 14+. Without it you get undefined references, not a
//                 worse trace. GCC 13 uses -lstdc++_libbacktrace.
//   -g            Without it, description() still works but source_file()
//                 is empty and source_line() is 0.
//   -O0           From -O1 up, inlining and tail calls collapse frames.

#include <chrono>
#include <cstdlib>
#include <new>
#include <iostream>
#include <print>
#include <stacktrace>
#include <stdexcept>
#include <string>

// =====================================================================
// 1. Capture the current stack and print it
// =====================================================================
void ShowWholeTrace()
{
    std::println("--- 1. capture and print ---");

    std::stacktrace trace = std::stacktrace::current();

    // A whole trace prints directly, via operator<< or std::format.
    std::println("{}", trace);
    std::println("size() = {}", trace.size());
    std::println("empty() = {}\n", trace.empty());
}

// =====================================================================
// 2. Look at one frame at a time: std::stacktrace_entry
// =====================================================================
void ShowEachEntry()
{
    std::println("--- 2. one entry at a time ---");

    std::stacktrace trace = std::stacktrace::current();

    for (const std::stacktrace_entry& entry : trace)
    {
        // An entry converts to bool. The last one is often empty.
        if (!entry)
            continue;

        std::println("  description : {}", entry.description());
        std::println("  source_file : {}", entry.source_file());
        std::println("  source_line : {}", entry.source_line());
        std::println("  address     : {:#x}", entry.native_handle());
        std::println("");
    }

    // Frames are indexable: [0] is innermost, and at() is bounds-checked.
    std::println("  innermost is {}\n", trace[0].description());
}

// =====================================================================
// 3. current(skip) and current(skip, max_depth)
// =====================================================================
void Helper()
{
    // Called from ShowSkipAndDepth, so compare the two captures here.
    std::stacktrace normal = std::stacktrace::current();
    std::stacktrace skipped = std::stacktrace::current(1);

    // skip = 1 drops Helper itself, so its caller becomes frame 0.
    // This is how a logging wrapper stays out of its own trace.
    std::println("  inside Helper, current()  frame 0 = {}",
                 normal[0].description());
    std::println("  inside Helper, current(1) frame 0 = {}",
                 skipped[0].description());
}

int Recurse(int depth)
{
    if (depth > 0)
        return Recurse(depth - 1);

    // max_depth caps the capture, which matters in deep recursion.
    std::stacktrace capped = std::stacktrace::current(0, 4);
    return static_cast<int>(capped.size());
}

void ShowSkipAndDepth()
{
    std::println("--- 3. skip and max_depth ---");

    Helper();

    std::println("  100 deep, current(0, 4) captured {} frames\n", Recurse(100));
}

// =====================================================================
// 4. The main use case: an exception that remembers where it was thrown
// =====================================================================
class TracedError : public std::runtime_error
{
public:
    explicit TracedError(const std::string& message)
        : std::runtime_error(message),
          // skip = 1 so the trace starts at the thrower, not at this ctor.
          thrown_at_(std::stacktrace::current(1))
    {
    }

    const std::stacktrace& ThrownAt() const noexcept { return thrown_at_; }

private:
    std::stacktrace thrown_at_;
};

void Innermost() { throw TracedError("disk is on fire"); }
void Middle()    { Innermost(); }
void Outermost() { Middle(); }

void ShowTracedException()
{
    std::println("--- 4. a trace captured at throw, printed at catch ---");

    try
    {
        Outermost();
    }
    catch (const TracedError& error)
    {
        std::println("  caught: {}", error.what());
        std::println("  thrown from:");
        std::println("{}\n", error.ThrownAt());
    }
}

// =====================================================================
// 5. Traces are comparable and hashable
// =====================================================================
void ShowComparison()
{
    std::println("--- 5. comparison and hashing ---");

    std::stacktrace here = std::stacktrace::current();
    std::stacktrace copy = here;
    std::stacktrace again = std::stacktrace::current();

    // A copy is equal. A second capture is not: it was taken on a
    // different line, so the return addresses differ.
    std::println("  copy == original     : {}", copy == here);
    std::println("  second capture equal : {}", again == here);
    std::println("  hash                 : {:#x}\n",
                 std::hash<std::stacktrace>{}(here));
}

// =====================================================================
// 6. What it costs. Run first, before anything warms the cache.
// =====================================================================
void ShowCost()
{
    using Clock = std::chrono::steady_clock;

    std::println("--- 6. cost (measured on a cold cache) ---");

    Clock::time_point t0 = Clock::now();
    std::stacktrace trace = std::stacktrace::current();
    Clock::time_point t1 = Clock::now();

    // The first symbolisation loads debug info for the whole binary.
    std::string first = trace[0].description();
    Clock::time_point t2 = Clock::now();

    std::string second = trace[0].description();
    Clock::time_point t3 = Clock::now();

    auto micros = [](Clock::time_point a, Clock::time_point b) {
        return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count();
    };

    std::println("  first  current()     : {} us", micros(t0, t1));
    std::println("  first  description() : {} us   <-- one-time", micros(t1, t2));
    std::println("  second description() : {} us", micros(t2, t3));
    std::println("");
    std::println("  Capturing is cheap. Printing your FIRST trace is not.\n");
}


// =====================================================================
// 7. How much does current() allocate?
// =====================================================================
// A global operator new: replacing this one function makes every "new" in
// the program come through here. That is an "allocator hook" - code on the
// allocation path. People install them to count allocations, find leaks, or
// route memory into a pool (LD_PRELOAD on malloc and custom std::allocators
// are the other common ways).
//
// Counting is off by default so the rest of the program is unaffected.
namespace counters
{
    inline int calls = 0;
    inline std::size_t bytes = 0;
    inline bool counting = false;
}

void* operator new(std::size_t size)
{
    if (counters::counting)
    {
        ++counters::calls;
        counters::bytes += size;
    }
    void* p = std::malloc(size);
    if (!p)
        throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

void ShowAllocation()
{
    std::println("--- 7. how much does current() allocate? ---");

    counters::calls = 0;
    counters::bytes = 0;
    counters::counting = true;
    std::stacktrace trace = std::stacktrace::current();
    counters::counting = false;

    std::println("  current() made {} allocation(s), {} bytes, for {} frames",
                 counters::calls, counters::bytes, trace.size());
    std::println("  sizeof(std::stacktrace_entry) = {} (just a return address)",
                 sizeof(std::stacktrace_entry));

    counters::calls = 0;
    counters::bytes = 0;
    counters::counting = true;
    std::stacktrace capped = std::stacktrace::current(0, 4);
    counters::counting = false;

    std::println("  current(0, 4) made {} allocation(s), {} bytes, for {} frames",
                 counters::calls, counters::bytes, capped.size());

    std::println("");
    std::println("  So current() is off-limits wherever allocation is: most");
    std::println("  obviously a signal handler. That is a rule about signal");
    std::println("  handlers, not about stacktrace - it applies equally to");
    std::println("  printf or std::string. See signal_reentrancy.c if you want");
    std::println("  to watch it fail.");
    std::println("");
}

int main()
{
    std::println("C++23 <stacktrace>");
    std::println("==================\n");

    ShowCost();              // first: the numbers are only honest when cold
    ShowWholeTrace();
    ShowEachEntry();
    ShowSkipAndDepth();
    ShowTracedException();
    ShowComparison();
    ShowAllocation();

    std::println("Rebuild to see what breaks:");
    std::println("  drop -g   -> source_file() empty, source_line() 0");
    std::println("  use -O1   -> frames collapse (inlining, tail calls)");
    std::println("  strip     -> descriptions go empty entirely");

    return 0;
}
