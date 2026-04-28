#include <chrono>
#include <print>
#include <thread>

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
        std::println("{}: {}", std::this_thread::get_id(), ++counter);
        std::this_thread::sleep_for(1s);
    }

    std::println("{}: Stopping PrintFunction at counter value: {}", std::this_thread::get_id(), counter);
}

void PrintFunctioWrapper()
{
    std::println("Entering PrintFunctionWrapper...");
    PrintFunction();
    std::println("Exiting PrintFunctionWrapper...");
}

void PrintFunctionWrapperWrapper()
{
    std::println("Entering PrintFunctionWrapperWrapper...");
    PrintFunctioWrapper();
    std::println("Exiting PrintFunctionWrapperWrapper...");
}

int main()
{
    std::println("=========================================");
    std::println("Process ID: {}", getpid());
    std::println("System is running. Attach GDB to inspect.");
    std::println("=========================================");
    PrintFunctionWrapperWrapper();
    return 0;
}