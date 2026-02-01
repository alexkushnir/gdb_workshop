#include <print>

class VeryImportantClass
{
public:
    VeryImportantClass() = default;
    ~VeryImportantClass() = default;

    void doSomething() 
    {
        std::println("Doing something");
    }
};

int main()
{
    VeryImportantClass obj;
    obj.doSomething();

    VeryImportantClass obj2;
    obj2.doSomething();

    std::println("Doing something else");
    return 0;
}