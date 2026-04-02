#include <chrono>
#include <cmath>
#include <iostream>
#include <print>
#include <thread>
#include <unistd.h>
#include <vector>

void MemorySpike(int durationSeconds)
{
    std::println("[Phase: SPIKE] Allocating ~600MB of memory...");

    // Allocate ~600MB (600 * 1024 * 1024 bytes)
    size_t numElements = 600 * 1024 * 1024;
    std::vector<char> buffer(numElements);

    // Fill the buffer to ensure memory is actually committed to RSS
    std::fill(buffer.begin(), buffer.end(), 1);

    std::println("[Phase: SPIKE] Memory allocated. Holding for {} seconds...", durationSeconds);
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));

    std::println("[Phase: SPIKE] Releasing memory...");
    buffer.clear();
    buffer.shrink_to_fit();
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("ProcDump Memory Spike Simulator (PID: {})", getpid());
    std::println("==========================================");
    std::println("Scenario: This process will simulate periodic heap growth.");
    std::println("Every 10 seconds, it will allocate 600MB for 5 seconds.");
    std::println("==========================================");

    while (true)
    {
        std::println("\n[Phase: Idle] Sleeping for 10 seconds...");
        std::this_thread::sleep_for(std::chrono::seconds(10));

        MemorySpike(5);
        std::println("[Phase: Recovered] Memory released.");
    }

    return 0;
}
