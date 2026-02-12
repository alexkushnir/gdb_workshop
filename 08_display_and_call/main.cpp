#include "AReallyImportantClass.h"
#include <print>

bool g_flag = false;

void DebugFunc()
{
    g_flag = true;
}

int main()
{
    AReallyImportantClass aric;
    std::println("Hello, World!");

    int a[10];
    
    for (auto& entry : a)
    {
        entry = 1;
    }
    
    while (!g_flag)
    {
        std::println("Still looping...");
    }
    return 0;
}
