#include "printer.h"
#include <print>

void PrintHello()
{
    std::println("Hello, world!");
    std::print(stderr, "error: {}\n", 42);
}
