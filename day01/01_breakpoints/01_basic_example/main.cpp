#include <iostream>

class VeryImportantClass
{
public:
    VeryImportantClass() = default;
    ~VeryImportantClass() = default;

    void DoSomething()
    {
        std::cout << "Doing something\n";
    }
};

int main()
{
    VeryImportantClass obj;
    obj.DoSomething();

    VeryImportantClass obj2;
    obj2.DoSomething();

    std::cout << "Doing something else\n";
    return 0;
}