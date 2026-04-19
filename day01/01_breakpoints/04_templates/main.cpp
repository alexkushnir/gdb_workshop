#include <print>
#include <string>
#include <typeinfo>
#include <vector>

template <typename T>
void PrintValue(const T& value)
{
    std::println("Value: {} (Type: {})", value, typeid(T).name());
}

template <typename T>
class Calculator
{
public:
    Calculator(T val) : m_value(val) {}

    void Add(T other)
    {
        m_value += other;
        std::println("Updated value: {}", m_value);
    }

private:
    T m_value;
};

int main()
{
    // Call template function with different types
    PrintValue(42);                 // instantiation with int
    PrintValue(3.14159);            // instantiation with double
    PrintValue(std::string("GDB")); // instantiation with std::string

    // Class templates
    Calculator<int> intCalc(10);
    intCalc.Add(5);

    Calculator<double> doubleCalc(10.5);
    doubleCalc.Add(2.3);

    return 0;
}
