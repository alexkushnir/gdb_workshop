#include "printer.h"
#include <iostream>
#include <algorithm>
#include <iterator>

int main(int argc, char* argv[])
{
    std::copy(&argv[0], &argv[argc], std::ostream_iterator<const char*>(std::cout, " "));
    PrintHello();
    return 0;
}
