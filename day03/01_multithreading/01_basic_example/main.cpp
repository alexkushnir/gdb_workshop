#include <atomic>
#include <chrono>
#include <functional>
#include <print>
#include <scn/scan.h>
#include <pthread.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std::chrono_literals;

std::atomic<int> sharedCounter{0};
std::vector<std::jthread> workerThreads;

// Task 1: Periodic increment of a shared counter
void CounterTask(std::stop_token stoken)
{
    pthread_setname_np(pthread_self(), "CounterThread");
    std::println("[CounterTask] Started.");
    while (!stoken.stop_requested())
    {
        sharedCounter++;
        std::this_thread::sleep_for(1s);
    }
    std::println("[CounterTask] Exiting.");
}

// Task 2: Calculate Fibonacci numbers (simulate computation)
void MathTask(std::stop_token stoken)
{
    pthread_setname_np(pthread_self(), "MathThread");
    std::println("[MathTask] Started.");
    long a = 0, b = 1;
    while (!stoken.stop_requested())
    {
        long next = a + b;
        a = b;
        b = next;
        if (a > 1000000) { a = 0; b = 1; } // Prevent overflow for demo
        std::this_thread::sleep_for(500ms);
    }
    std::println("[MathTask] Exiting.");
}

// Task 3: Periodic status report
void StatusTask(std::stop_token stoken)
{
    pthread_setname_np(pthread_self(), "StatusThread");
    std::println("[StatusTask] Started.");
    while (!stoken.stop_requested())
    {
        std::println("[StatusTask] System healthy. Shared counter: {}", sharedCounter.load());
        std::this_thread::sleep_for(5s);
    }
    std::println("[StatusTask] Exiting.");
}

// Task 4: Idle/Waiting thread
void IdleTask(std::stop_token stoken)
{
    pthread_setname_np(pthread_self(), "IdleThread");
    std::println("[IdleTask] Started (mostly sleeping).");
    while (!stoken.stop_requested())
    {
        std::this_thread::sleep_for(10s);
    }
    std::println("[IdleTask] Exiting.");
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("Multithreading 'Attach' Demo");
    std::println("============================");
    std::println("PID: {}", getpid());
    std::println("\n[Main] Spawning distinct worker threads...");

    workerThreads.emplace_back(CounterTask);
    workerThreads.emplace_back(MathTask);
    workerThreads.emplace_back(StatusTask);
    workerThreads.emplace_back(IdleTask);
    
    pthread_setname_np(pthread_self(), "MainThread");
    
    std::println("[Main] All threads running. You can now attach GDB.");
    std::println("[Main] Example: gdb -p {}", getpid());
    std::println("[Main] Then try: info threads, thread apply all bt");

    std::println("\nPress Enter to stop all threads and exit...");
    auto result = scn::prompt<std::string>("", "{}");

    std::println("\n[Main] Requesting stop for all threads...");
    // jthreads will automatically stop and join on destruction (when vector is cleared/destroyed)
    workerThreads.clear();

    std::println("[Main] All worker threads joined. Exiting.");

    return 0;
}
