#include <iostream>
#include <string>

// Function that will cause a crash.
void CrashHere(int* ptr)
{
    std::cout << "Attempting to dereference pointer...\n";
    *ptr = 42; // This will crash if ptr is null
}

void MiddleFunction(int* ptr)
{
    CrashHere(ptr);
}

int main()
{
    std::cout << "Crashing program starting...\n";
    int* myPtr = nullptr;
    MiddleFunction(myPtr);
    return 0;
}
