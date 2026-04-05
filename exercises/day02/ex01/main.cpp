#include <iostream>
#include <print>
#include <string>

// Function that will cause a crash.
void CrashHere(int* ptr)
{
    std::println("Attempting to dereference pointer...");
    *ptr = 42; // This will crash if ptr is null
}

void MiddleFunction(int* ptr)
{
    CrashHere(ptr);
}

int main()
{
    std::println("Crashing program starting...");
    int* myPtr = nullptr;
    MiddleFunction(myPtr);
    return 0;
}
