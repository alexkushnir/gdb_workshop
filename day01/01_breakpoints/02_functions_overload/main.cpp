#include <iostream>
#include <string>

/**
 * Overloaded functions for GDB breakpoint demonstration.
 */

void PrintMessage(int value)
{
    std::cout << "[int] Message: " << value << std::endl;
}

void PrintMessage(double value)
{
    std::cout << "[double] Message: " << value << std::endl;
}

void PrintMessage(const std::string& message)
{
    std::cout << "[string] Message: " << message << std::endl;
}

namespace utils
{
    void PrintMessage(long value)
    {
        std::cout << "[long] Message from namespace utils: " << value << std::endl;
    }
} // namespace utils

int main()
{
    std::cout << "GDB Overloaded Functions Demonstration" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    PrintMessage(42);
    PrintMessage(3.14159);
    PrintMessage(std::string("Hello from GDB workshop!"));

    utils::PrintMessage(123456789L);

    return 0;
}
