#include "printer.h"
#include <iostream>
#include <ranges>

int main(int argc, char* argv[])
{
    std::ranges::copy(&argv[0], &argv[argc], std::ostream_iterator<const char*>(std::cout, " "));
    PrintHello();
    return 0;
}
