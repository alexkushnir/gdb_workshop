#include <iostream>
#include <vector>
#include <print>

// Recursive function to show off the call stack in VS Code UI.
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// A more complex function to show off the Variables and Watch view.
struct Point {
    int x;
    int y;
};

void transform_points(std::vector<Point>& points) {
    for (auto& p : points) {
        p.x *= 2;
        p.y += 10;
    }
}

int main() {
    int n = 8;
    std::println("Calculating Fibonacci sequence up to {}:", n);

    for (int i = 0; i <= n; ++i) {
        int result = fibonacci(i);
        std::println("fib({}) = {}", i, result);
    }

    std::vector<Point> points = {{1, 2}, {3, 4}, {5, 6}};
    std::println("Transforming points...");
    transform_points(points);

    for (const auto& p : points) {
        std::println("Point: ({}, {})", p.x, p.y);
    }

    return 0;
}
