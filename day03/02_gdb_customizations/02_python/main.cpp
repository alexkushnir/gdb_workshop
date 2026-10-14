#include <iostream>
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
    std::cout << "Processing: " << p.m_name << " (Age: " << p.m_age << ")\n";
}

int main()
{
    std::cout << "GDB Python Extension Demo\n";
    std::cout << "=========================\n";

    Workshop::Person alice{"Alice", 30, true};
    Workshop::Person bob{"Bob", 25, false};

    int iterationCount = 5;
    double pi = 3.14159;
    std::string message = "Hello from GDB Workshop!";

    ProcessPerson(alice);
    ProcessPerson(bob);

    std::cout << "Demo finished.\n";
    return 0;
}
