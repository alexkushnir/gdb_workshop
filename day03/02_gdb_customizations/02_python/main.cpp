#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace Workshop
{
    struct Person
    {
        std::string m_name;
        int m_age;
        bool m_isActive;
    };
} // namespace Workshop

void ProcessPerson(const Workshop::Person& p)
{
    std::println("Processing: {} (Age: {})", p.m_name, p.m_age);
}

int main()
{
    std::println("GDB Python Extension Demo");
    std::println("=========================");

    Workshop::Person alice{"Alice", 30, true};
    Workshop::Person bob{"Bob", 25, false};

    int iterationCount = 5;
    double pi = 3.14159;
    std::string message = "Hello from GDB Workshop!";

    ProcessPerson(alice);
    ProcessPerson(bob);

    std::println("Demo finished.");
    return 0;
}
