#include <iostream>
#include <stacktrace>

void FunctionC()
{
    std::cout << "Stacktrace from functionC:\n\n";
    auto st = std::stacktrace::current();
    std::cout << st << '\n';

    std::cout << "--- Demonstrating std::stacktrace_entry ---\n";
    for (const std::stacktrace_entry& entry : st)
    {
        std::cout << "Function: " << entry.description() << '\n'
                  << "    File: " << entry.source_file() << '\n'
                  << "    Line: " << entry.source_line() << "\n\n";
    }
}

void FunctionB()
{
    FunctionC();
}

void FunctionA()
{
    FunctionB();
}

int main()
{
    std::cout << "Demonstrating C++23 <stacktrace> Header\n";
    std::cout << "========================================\n\n";

    FunctionA();

    return 0;
}
