#include "lib.h"

#include <print>

namespace advanced_breakpoints 
{
    void VeryImportantClass::DoSomething()
    {
        for (auto i = 0; i < 10co; ++i)
        {
            std::println("Doing something important at {} iteration", i);
        }
    }
}