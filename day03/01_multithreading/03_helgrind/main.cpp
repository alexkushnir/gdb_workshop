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
    std::string name = "Worker-" + std::to_string(id);
    pthread_setname_np(pthread_self(), name.c_str());

    for (int i = 0; i < 100000; ++i)
    {
        // Data race occurs here
        sharedCounter++;
    }
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("Helgrind Data Race Demo (PID: {})", getpid());
    std::println("========================================");
    std::println("Spawning 2 threads to increment a shared counter WITHOUT synchronization.");

    std::vector<std::jthread> threads;
    for (int i = 0; i < 2; ++i)
    {
        threads.emplace_back(WorkerFunc, i);
    }

    std::println("Threads started. Helgrind should detect several data races shortly.");

    return 0;
}
