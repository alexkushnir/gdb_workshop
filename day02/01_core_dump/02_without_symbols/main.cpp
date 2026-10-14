#include <iostream>
#include <string>

// Simple nested call example that ends in a deliberate null-pointer crash.
// This file is the "no symbols" variant: the CMakeLists will build with
// debug info and then strip it (copying the .debug file to symbols/).

void Crash(int intVar, std::string strVar)
{
    std::cout << "In Crash(): intVar=" << intVar << " strVar=" << strVar << "\n";
    int* a = nullptr;
    *a = 1; // deliberate crash
}

void CrashWrapper(int intVar, std::string strVar)
{
    intVar++;
    strVar += " World";
    Crash(intVar, strVar);
}

void CrashWrapperWrapper(int intVar, std::string strVar)
{
    CrashWrapper(intVar, strVar);
}

int main()
{
    int intVar = 5;
    std::string strVar = "Hello";
    std::cout << "Starting crash demo: " << intVar << " " << strVar << "\n";
    CrashWrapperWrapper(intVar, strVar);
    return 0;
}