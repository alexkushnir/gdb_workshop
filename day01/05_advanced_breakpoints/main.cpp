#include "lib.h"
#include <dlfcn.h>
#include <iostream>

#include <filesystem>

int main()
{
    advanced_breakpoints::VeryImportantClass().DoSomething();
    return 0;
}