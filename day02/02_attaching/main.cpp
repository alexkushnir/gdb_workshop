#include <chrono>
#include <print>
#include <thread>

using namespace std::chrono_literals;

void PrintFunction()
{
    std::uint64_t counter = 0;
    while (true)
    {
        std::println("{}: {}", std::this_thread::get_id(), ++counter);
        std::this_thread::sleep_for(1s);
    }
    
    std::println("Loop finished.");
}

void PrintFunctioWrapper()
{
    PrintFunction();
}

void PrintFunctionWrapperWrapper()
{
    PrintFunctioWrapper();
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