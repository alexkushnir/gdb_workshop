#include <iostream>
#include <limits>

void IntegerOverflow()
{
    std::cout << "\n--- Triggering Signed Integer Overflow ---\n";
    int val = std::numeric_limits<int>::max();
    std::cout << "Current max int: " << val << "\n";
    int overflowed = val + 1; // Undefined Behavior
    std::cout << "Overflowed result: " << overflowed << "\n";
}

void ShiftOutOfBounds()
{
    std::cout << "\n--- Triggering Shift Out of Bounds ---\n";
    int val = 1;
    int shift = 32;            // Assuming 32-bit int
    int result = val << shift; // Undefined Behavior
    std::cout << "Shift result: " << result << "\n";
}

struct Data
{
    int m_value;
    void Print()
    {
        std::cout << "Value: " << m_value << "\n";
    }
};

void NullPointerDereference()
{
    std::cout << "\n--- Triggering Null Pointer Dereference ---\n";
    Data* ptr = nullptr;
    // Calling a member function on nullptr is Undefined Behavior
    // (Even if 'this' isn't used immediately, UBSan catches the call)
    ptr->Print();
}

void DivisionByZero(int denominator)
{
    std::cout << "\n--- Triggering Division by Zero ---\n";
    int result = 100 / denominator; // Undefined Behavior if denominator is 0
    std::cout << "Result: " << result << "\n";
}

int main(int argc, char** argv)
{
    std::cout << "UndefinedBehaviorSanitizer (UBSan) Demo\n";
    std::cout << "========================================\n";

    IntegerOverflow();
    ShiftOutOfBounds();
    DivisionByZero(0);

    NullPointerDereference();

    std::cout << "\nDemo finished (if you see this, UBSan didn't halt execution).\n";
    return 0;
}
