#include <iostream>
#include <print>
#include <string>
#include <vector>

#include <memory>

long Fibonacci(int n)
{
    if (n <= 1)
        return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

struct Node
{
    int m_data;
    std::unique_ptr<Node> m_next;
};

void PrintList(const Node* head)
{
    const Node* current = head;
    while (current)
    {
        std::println("Node value: {}", current->m_data);
        current = current->m_next.get();
    }
}

int main()
{
    std::println("GDB Dashboard Demo");
    std::println("==================");

    // 1. Numeric state
    int n = 7;
    long result = Fibonacci(n);
    std::println("Fibonacci({}) = {}", n, result);

    // 2. Vector state
    std::vector<std::string> colors = {"Red", "Green", "Blue", "Yellow"};
    for (const auto& color : colors)
    {
        std::println("Color: {}", color);
    }

    // 3. Smart Pointer/List state
    auto n1 = std::make_unique<Node>(10, nullptr);
    n1->m_next = std::make_unique<Node>(20, nullptr);
    n1->m_next->m_next = std::make_unique<Node>(30, nullptr);

    PrintList(n1.get());

    return 0;
}
