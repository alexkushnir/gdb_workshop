#include "lib.h"
#include <dlfcn.h>
#include <print>

#include <filesystem>

int main()
{
    advanced_breakpoints::VeryImportantClass().DoSomething();
    return 0;
}