#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

void PrintFunction()
{
    std::uint64_t counter = 0;
    while (true)
    {
        std::cout << std::this_thread::get_id() << ": " << ++counter << "\n";
        std::this_thread::sleep_for(1s);
    }
    
    std::cout << "Loop finished.\n";
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
    std::cout << "=========================================\n";
    std::cout << "Process ID: " << getpid() << "\n";
    std::cout << "System is running. Attach GDB to inspect.\n";
    std::cout << "=========================================\n";
    PrintFunctionWrapperWrapper();
    return 0;
}