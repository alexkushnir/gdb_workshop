#include <iostream>
#include <print>
#include <vector>

// Recursive function to show off the call stack in VS Code UI.
int Fibonacci(int n)
{
    if (n <= 1)
    {
        return n;
    }
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

// A more complex function to show off the Variables and Watch view.
struct Point
{
    int m_x;
    int m_y;
};

void TransformPoints(std::vector<Point>& points)
{
    for (auto& p : points)
    {
        p.m_x *= 2;
        p.m_y += 10;
    }
}

int main()
{
    int n = 8;
    std::println("Calculating Fibonacci sequence up to {}:", n);

    for (int i = 0; i <= n; ++i)
    {
        int result = Fibonacci(i);
        std::println("fib({}) = {}", i, result);
    }

    std::vector<Point> points = {{1, 2}, {3, 4}, {5, 6}};
    std::println("Transforming points...");
    TransformPoints(points);

    for (const auto& p : points)
    {
        std::println("Point: ({}, {})", p.m_x, p.m_y);
    }

    return 0;
}
