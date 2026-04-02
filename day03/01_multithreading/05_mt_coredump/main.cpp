#include <chrono>
#include <print>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std::chrono_literals;

void WorkerTask(int id)
{
    std::string name = "Worker-" + std::to_string(id);
    pthread_setname_np(pthread_self(), name.c_str());

    std::println("[{}] Started. Doing some work...", name);

    int counter = 0;
    while (true)
    {
        counter++;
        std::this_thread::sleep_for(1s);
        if (counter % 5 == 0)
        {
            std::println("[{}] Heartbeat: counter is {}", name, counter);
        }
    }
}

void CrashingTask()
{
    pthread_setname_np(pthread_self(), "CrashingThread");
    std::println("[CrashingThread] Started. I will crash in 3 seconds...");

    std::this_thread::sleep_for(3s);

    std::println("[CrashingThread] Triggering null pointer dereference now!");

    int* ptr = nullptr;
    *ptr = 42; // BOOM!

    std::println("[CrashingThread] This should never be printed.");
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("Multithreaded Core Dump Demo (PID: {})", getpid());
    std::println("==========================================");
    std::println("Spawning 3 worker threads and 1 thread that will crash.");

    pthread_setname_np(pthread_self(), "MainThread");

    std::vector<std::jthread> workers;
    for (int i = 1; i <= 3; ++i)
    {
        workers.emplace_back(WorkerTask, i);
    }

    std::jthread crasher(CrashingTask);

    while (true)
    {
        std::this_thread::sleep_for(1s);
    }

    return 0;
}
