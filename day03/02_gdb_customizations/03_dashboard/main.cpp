#include <iostream>
#include <string>
#include <vector>

#include <memory>
#include <utility>

long Fibonacci(int n)
{
    if (n <= 1)
        return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

struct Node
{
    Node(int data, std::unique_ptr<Node> next) : m_data(data), m_next(std::move(next)) {}

    int m_data;
    std::unique_ptr<Node> m_next;
};

void PrintList(const Node* head)
{
    const Node* current = head;
    while (current)
    {
        std::cout << "Node value: " << current->m_data << "\n";
        current = current->m_next.get();
    }
}

int main()
{
    std::cout << "GDB Dashboard Demo\n";
    std::cout << "==================\n";

    // 1. Numeric state
    int n = 7;
    long result = Fibonacci(n);
    std::cout << "Fibonacci(" << n << ") = " << result << "\n";

    // 2. Vector state
    std::vector<std::string> colors = {"Red", "Green", "Blue", "Yellow"};
    for (const auto& color : colors)
    {
        std::cout << "Color: " << color << "\n";
    }

    // 3. Smart Pointer/List state
    auto n1 = std::make_unique<Node>(10, nullptr);
    n1->m_next = std::make_unique<Node>(20, nullptr);
    n1->m_next->m_next = std::make_unique<Node>(30, nullptr);

    PrintList(n1.get());

    return 0;
}
