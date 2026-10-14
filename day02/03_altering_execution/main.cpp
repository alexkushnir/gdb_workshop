#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

bool StopStatus(bool shouldToggle)
{
    static bool shouldStop = false;
    if (shouldToggle)
    {
        shouldStop = !shouldStop;
    }

    return shouldStop;
}

bool StopStatus()
{
    return StopStatus(true);
}

void PrintFunction()
{
    std::uint64_t counter = 0;
    while (!StopStatus(false) && counter < 1'000'000)
    {
        std::cout << std::this_thread::get_id() << ": " << ++counter << "\n";
        std::this_thread::sleep_for(1s);
    }

    std::cout << std::this_thread::get_id() << ": Stopping PrintFunction at counter value: " << counter << "\n";
}

void PrintFunctioWrapper()
{
    std::cout << "Entering PrintFunctionWrapper...\n";
    PrintFunction();
    std::cout << "Exiting PrintFunctionWrapper...\n";
}

void PrintFunctionWrapperWrapper()
{
    std::cout << "Entering PrintFunctionWrapperWrapper...\n";
    PrintFunctioWrapper();
    std::cout << "Exiting PrintFunctionWrapperWrapper...\n";
}

int main()
{
    std::cout << "=========================================\n";
    std::cout << "Process ID: " << getpid() << "\n";
    std::cout << "System is running. Attach GDB to inspect.\n";
    std::cout << "=========================================\n";
    PrintFunctionWrapperWrapper();
    return 0;
}