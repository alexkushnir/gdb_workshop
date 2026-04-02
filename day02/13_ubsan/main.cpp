#include <iostream>
#include <limits>
#include <print>

void IntegerOverflow()
{
    std::println("\n--- Triggering Signed Integer Overflow ---");
    int val = std::numeric_limits<int>::max();
    std::println("Current max int: {}", val);
    int overflowed = val + 1; // Undefined Behavior
    std::println("Overflowed result: {}", overflowed);
}

void ShiftOutOfBounds()
{
    std::println("\n--- Triggering Shift Out of Bounds ---");
    int val = 1;
    int shift = 32;            // Assuming 32-bit int
    int result = val << shift; // Undefined Behavior
    std::println("Shift result: {}", result);
}

struct Data
{
    int m_value;
    void Print()
    {
        std::println("Value: {}", m_value);
    }
};

void NullPointerDereference()
{
    std::println("\n--- Triggering Null Pointer Dereference ---");
    Data* ptr = nullptr;
    // Calling a member function on nullptr is Undefined Behavior
    // (Even if 'this' isn't used immediately, UBSan catches the call)
    ptr->Print();
}

void DivisionByZero(int denominator)
{
    std::println("\n--- Triggering Division by Zero ---");
    int result = 100 / denominator; // Undefined Behavior if denominator is 0
    std::println("Result: {}", result);
}

int main(int argc, char** argv)
{
    std::println("UndefinedBehaviorSanitizer (UBSan) Demo");
    std::println("========================================");

    IntegerOverflow();
    ShiftOutOfBounds();
    DivisionByZero(0);

    // Note: Null pointer deref might crash the app after UBSan reports it,
    // so we put it last or use UBSAN_OPTIONS=halt_on_error=0.
    NullPointerDereference();

    std::println("\nDemo finished (if you see this, UBSan didn't halt execution).");
    return 0;
}
