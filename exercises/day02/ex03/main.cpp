#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std::literals;

int main()
{
    pid_t pid = getpid();
    int counter = 0;

    std::cout << "=== Attach to Process Exercise ===\n";
    std::cout << "My Process ID (PID) is: " << pid << "\n";
    std::cout << "I'm going into a loop now. Try attaching GDB to me!\n";

    while (true)
    {
        counter++;
        std::cout << "Current counter: " << counter << " (PID: " << pid << ")\n";
        std::this_thread::sleep_for(2s);
    }

    return 0;
}
