#include <print>

class VeryImportantClass
{
public:
    VeryImportantClass() = default;
    ~VeryImportantClass() = default;

    void DoSomething()
    {
        std::println("Doing something");
    }
};

int main()
{
    VeryImportantClass obj;
    obj.DoSomething();

    VeryImportantClass obj2;
    obj2.DoSomething();

    std::println("Doing something else");
    return 0;
}