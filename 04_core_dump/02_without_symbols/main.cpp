#include <string>

void Crash(int intVar, std::string strVar)
{
    int* a = nullptr;
    *a = 1;
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
    CrashWrapperWrapper(intVar, strVar);
    return 0;
}