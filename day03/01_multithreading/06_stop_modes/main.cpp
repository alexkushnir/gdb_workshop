#include <chrono>
#include <iostream>
#include <mutex>
#include <print>
#include <pthread.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

/**
 * GDB STOP MODES DEMONSTRATION
 * ============================
 *
 * This project demonstrates the difference between "All-Stop" and "Non-Stop" modes in GDB.
 *
 * --- ALL-STOP MODE (Default) ---
 * When any thread hits a breakpoint, GDB stops ALL threads in the process.
 *
 * Instructions:
 * 1. gdb ./stop_modes
 * 2. (gdb) break WorkerFunc
 * 3. (gdb) run
 * 4. Notice that when one thread hits the breakpoint, all output from other threads stops.
 * 5. (gdb) info threads  --> All threads will be in 'stopped' state.
 *
 * --- NON-STOP MODE ---
 * When a thread hits a breakpoint, ONLY that thread stops. Others continue running.
 *
 * Instructions:
 * 1. gdb ./stop_modes
 * 2. (gdb) set non-stop on
 * 3. (gdb) set mi-async on
 * 4. (gdb) break WorkerAlpha
 * 5. (gdb) run &
 * 6. Observe: Thread 2 (Worker-Alpha) hits the breakpoint and stops.
 * 7. (gdb) info threads
 *    Notice that Thread 2 is 'stopped', while 1, 3, 4 are still '(running)'!
 * 8. Observe the output: Worker-Beta and Worker-Gamma continue printing!
 * 9. (gdb) thread 2
 * 10. (gdb) continue &
 */

std::mutex coutMutex;

void WorkerAlpha()
{
    int counter = 0;
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::println("[Worker-Alpha] Loop iteration: {}", counter++);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void WorkerBeta()
{
    int counter = 0;
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::println("[Worker-Beta]  Processing step: {}", counter++);
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void WorkerGamma()
{
    int counter = 0;
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::println("[Worker-Gamma] Periodic check: {}", counter++);
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main()
{
    std::println("Starting multithreaded stop-modes demonstration...");
    std::println("Process ID: {}", getpid());

    std::vector<std::thread> threads;

    // Launch different functions in each thread
    threads.emplace_back(WorkerAlpha);
    pthread_setname_np(threads.back().native_handle(), "Worker-Alpha");

    threads.emplace_back(WorkerBeta);
    pthread_setname_np(threads.back().native_handle(), "Worker-Beta");

    threads.emplace_back(WorkerGamma);
    pthread_setname_np(threads.back().native_handle(), "Worker-Gamma");

    for (auto& t : threads)
    {
        t.join();
    }

    return 0;
}
