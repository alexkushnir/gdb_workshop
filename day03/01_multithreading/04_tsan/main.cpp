#include <iostream>
#include <print>
#include <pthread.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

// Shared variable without synchronization to trigger a data race
int sharedCounter = 0;

void WorkerFunc(int id)
{
    // Set thread name for clearer TSan reports
    std::string threadName = "Worker-" + std::to_string(id);
    pthread_setname_np(pthread_self(), threadName.c_str());

    std::println("[{}] Started.", threadName);

    for (int i = 0; i < 100000; ++i)
    {
        // Data race occurs here
        sharedCounter++;
    }

    std::println("[{}] Finished.", threadName);
}

int main()
{
    // Disable buffering for stdout to ensure we see prints before TSan reports
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("ThreadSanitizer (TSan) Demo (PID: {})", getpid());
    std::println("==========================================");
    std::println("Spawning 2 threads to increment a shared counter WITHOUT synchronization.");

    std::vector<std::jthread> threads;
    for (int i = 0; i < 2; ++i)
    {
        threads.emplace_back(WorkerFunc, i);
    }

    return 0;
}
