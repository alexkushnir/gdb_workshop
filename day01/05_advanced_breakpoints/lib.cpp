#include "lib.h"

#include <iostream>

namespace advanced_breakpoints
{
    void VeryImportantClass::DoSomething()
    {
        for (auto i = 0; i < 10; ++i)
        {
            std::cout << "Doing something important at " << i << " iteration\n";
        }
    }
} // namespace advanced_breakpoints