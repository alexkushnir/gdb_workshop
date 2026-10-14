#include <chrono>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std::chrono_literals;

void WorkerTask(int id)
{
    std::string name = "Worker-" + std::to_string(id);
    pthread_setname_np(pthread_self(), name.c_str());

    std::cout << "[" << name << "] Started. Doing some work...\n";

    int counter = 0;
    while (true)
    {
        counter++;
        std::this_thread::sleep_for(1s);
        if (counter % 5 == 0)
        {
            std::cout << "[" << name << "] Heartbeat: counter is " << counter << "\n";
        }
    }
}

void CrashingTask()
{
    pthread_setname_np(pthread_self(), "CrashingThread");
    std::cout << "[CrashingThread] Started. I will crash in 3 seconds...\n";

    std::this_thread::sleep_for(3s);

    std::cout << "[CrashingThread] Triggering null pointer dereference now!\n";

    int* ptr = nullptr;
    *ptr = 42; // BOOM!

    std::cout << "[CrashingThread] This should never be printed.\n";
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::cout << "Multithreaded Core Dump Demo (PID: " << getpid() << ")\n";
    std::cout << "==========================================\n";
    std::cout << "Spawning 3 worker threads and 1 thread that will crash.\n";

    pthread_setname_np(pthread_self(), "MainThread");

    std::vector<std::thread> workers;
    for (int i = 1; i <= 3; ++i)
    {
        workers.emplace_back(WorkerTask, i);
    }

    std::thread crasher(CrashingTask);

    while (true)
    {
        std::this_thread::sleep_for(1s);
    }

    return 0;
}
