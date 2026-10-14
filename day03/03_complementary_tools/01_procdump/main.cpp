#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

void MemorySpike(int durationSeconds)
{
    std::cout << "[Phase: SPIKE] Allocating ~600MB of memory...\n";

    // Allocate ~600MB (600 * 1024 * 1024 bytes)
    size_t numElements = 600 * 1024 * 1024;
    std::vector<char> buffer(numElements);

    // Fill the buffer to ensure memory is actually committed to RSS
    std::fill(buffer.begin(), buffer.end(), 1);

    std::cout << "[Phase: SPIKE] Memory allocated. Holding for " << durationSeconds << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));

    std::cout << "[Phase: SPIKE] Releasing memory...\n";
    buffer.clear();
    buffer.shrink_to_fit();
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::cout << "ProcDump Memory Spike Simulator (PID: " << getpid() << ")\n";
    std::cout << "=============================================================\n";
    std::cout << "Scenario: This process will simulate periodic heap growth.\n";
    std::cout << "Every 10 seconds, it will allocate 600MB for 5 seconds.\n";
    std::cout << "=============================================================\n";

    while (true)
    {
        std::cout << "\n[Phase: Idle] Sleeping for 10 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(10));

        MemorySpike(10);
        std::cout << "[Phase: Recovered] Memory released.\n";
    }

    return 0;
}
