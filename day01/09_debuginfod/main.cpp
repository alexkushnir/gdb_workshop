// main.cpp — Debuginfod demo
//
// A deliberately non-trivial program so that GDB has interesting things to
// show when it fetches debug information via debuginfod:
//   - multiple functions with local variables
//   - a recursive algorithm (Fibonacci)
//   - a small container walk that the out-of-bounds InnerFunction will crash
//
// BUILD / STRIP WORKFLOW (see build.sh and CMakeLists.txt):
//   1. Build with -ggdb3 → full DWARF ends up in the binary.
//   2. objcopy --only-keep-debug  → separate .debug file
//   3. strip --strip-debug        → stripped binary
//   4. serve the .debug file with  debuginfod  (or elfutils debuginfod-find)
//   5. Set DEBUGINFOD_URLS=http://localhost:8002 before launching GDB
//   → GDB will download the DWARF on demand and show full source / variables.

#include <print>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

// ── Fibonacci (recursive, on purpose — gives a deep call stack in GDB) ────────
static long Fibonacci(int n)
{
    if (n <= 1)
        return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

// ── Inner / outer functions (mirrors 03_preparing_the_executable) ─────────────
static void InnerFunction(std::vector<int>& vec, std::size_t idx)
{
    // Intentionally uses operator[] without bounds checking so that
    // an out-of-range index produces undefined behaviour — good for watchpoints.
    vec[idx] = 99;
}

static void OuterFunction(int size)
{
    std::vector<int> vec(static_cast<std::size_t>(size));
    for (int i = 0; i < size; ++i)
        InnerFunction(vec, static_cast<std::size_t>(i));

    // One call that goes one past the end — triggers UB / potential crash when
    // run under ASan or via GDB's catchpoint for SIGSEGV.
    InnerFunction(vec, static_cast<std::size_t>(size));
}

// ── Process a span and print a small report ───────────────────────────────────
static void ProcessData(std::span<const long> data)
{
    long sum = 0;
    for (long v : data)
        sum += v;
    std::println("  data size={}, sum={}, average={:.2f}",
                 data.size(),
                 sum,
                 static_cast<double>(sum) / static_cast<double>(data.size()));
}

// ── main ──────────────────────────────────────────────────────────────────────
int main()
{
    std::println("=== Debuginfod demo ===");

    // Collect some Fibonacci numbers so GDB can inspect them.
    constexpr int kCount = 12;
    std::vector<long> fibs;
    fibs.reserve(kCount);
    std::println("First {} Fibonacci numbers:", kCount);
    for (int i = 0; i < kCount; ++i)
    {
        long f = Fibonacci(i);
        fibs.push_back(f);
        std::println("  fib({}) = {}", i, f);
    }

    ProcessData(fibs);

    // This call intentionally writes one element past the end of the vector.
    // Run the stripped binary under GDB with DEBUGINFOD_URLS set to see full
    // source information fetched automatically from the debuginfod server.
    std::println("\nCalling OuterFunction(5) — last InnerFunction call is out-of-bounds:");
    OuterFunction(5);

    return 0;
}
