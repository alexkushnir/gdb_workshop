#include <iostream>
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
    std::cout << "Calculating Fibonacci sequence up to " << n << ":\n";

    for (int i = 0; i <= n; ++i)
    {
        int result = Fibonacci(i);
        std::cout << "fib(" << i << ") = " << result << "\n";
    }

    std::vector<Point> points = {{1, 2}, {3, 4}, {5, 6}};
    std::cout << "Transforming points...\n";
    TransformPoints(points);

    for (const auto& p : points)
    {
        std::cout << "Point: (" << p.m_x << ", " << p.m_y << ")\n";
    }

    return 0;
}
