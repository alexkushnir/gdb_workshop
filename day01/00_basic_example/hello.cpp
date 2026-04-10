#include "printer.h"
#include <ranges>
#include <iostream>

int main(int argc, char* argv[])
{
    std::ranges::copy(&argv[0], &argv[argc], 
        std::ostream_iterator<const char*>(std::cout, " "));
    PrintHello();
    return 0;
}
