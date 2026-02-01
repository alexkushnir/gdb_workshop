#include "lib.h"
#include <dlfcn.h>

int main()
{
    advanced_breakpoints::VeryImportantClass veryImportantClass;
    // veryImportantClass.DoSomething();
    dlopen("./libadvanced_breakpoints_lib.so", RTLD_LAZY);
    return 0;
}