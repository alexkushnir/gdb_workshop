#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <scn/scan.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std::chrono_literals;

std::atomic<int> sharedCounter{0};
std::atomic<bool> stopRequested{false};
std::vector<std::thread> workerThreads;

// Task 1: Periodic increment of a shared counter
void CounterTask()
{
    pthread_setname_np(pthread_self(), "CounterThread");
    std::cout << "[CounterTask] Started.\n";
    while (!stopRequested)
    {
        sharedCounter++;
        std::this_thread::sleep_for(1s);
        std::cout << "[CounterTask] Incremented counter to " << sharedCounter.load() << "\n";
    }
    std::cout << "[CounterTask] Exiting.\n";
}

// Task 2: Calculate Fibonacci numbers (simulate computation)
void MathTask()
{
    pthread_setname_np(pthread_self(), "MathThread");
    std::cout << "[MathTask] Started.\n";
    long a = 0, b = 1;
    while (!stopRequested)
    {
        long next = a + b;
        a = b;
        b = next;
        if (a > 1000000)
        {
            a = 0;
            b = 1;
        } // Prevent overflow for demo
        std::this_thread::sleep_for(500ms);
        std::cout << "[MathTask] Fibonacci number: " << a << "\n";
    }
    std::cout << "[MathTask] Exiting.\n";
}

// Task 3: Periodic status report
void StatusTask()
{
    pthread_setname_np(pthread_self(), "StatusThread");
    std::cout << "[StatusTask] Started.\n";
    while (!stopRequested)
    {
        std::cout << "[StatusTask] System healthy. Shared counter: " << sharedCounter.load() << "\n";
        std::this_thread::sleep_for(5s);
    }
    std::cout << "[StatusTask] Exiting.\n";
}

// Task 4: Idle/Waiting thread
void IdleTask()
{
    pthread_setname_np(pthread_self(), "IdleThread");
    std::cout << "[IdleTask] Started (mostly sleeping).\n";
    while (!stopRequested)
    {
        std::this_thread::sleep_for(10s);
        std::cout << "[IdleTask] Still idle...\n";
    }
    std::cout << "[IdleTask] Exiting.\n";
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::cout << "Multithreading 'Attach' Demo\n";
    std::cout << "============================\n";
    std::cout << "PID: " << getpid() << "\n";
    std::cout << "\n[Main] Spawning distinct worker threads...\n";

    workerThreads.emplace_back(CounterTask);
    workerThreads.emplace_back(MathTask);
    workerThreads.emplace_back(StatusTask);
    workerThreads.emplace_back(IdleTask);

    pthread_setname_np(pthread_self(), "MainThread");

    std::cout << "[Main] All threads running. You can now attach GDB.\n";
    std::cout << "[Main] Example: gdb -p " << getpid() << "\n";
    std::cout << "[Main] Then try: info threads, thread apply all bt\n";

    std::cout << "\nPress Enter to stop all threads and exit...\n";
    auto result = scn::prompt<std::string>("", "{}");

    std::cout << "\n[Main] Requesting stop for all threads...\n";
    stopRequested = true;
    for (auto& t : workerThreads)
    {
        t.join();
    }
    workerThreads.clear();

    std::cout << "[Main] All worker threads joined. Exiting.\n";

    return 0;
}
