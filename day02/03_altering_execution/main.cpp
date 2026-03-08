#include <print>
#include <string>

bool g_status = false;

int multiply(int a, int b) {
    int result = a * b;
    std::println("Multiplying {} by {}...", a, b);
    return result;
}

void hidden_function() {
    std::println("\n[SECRET] You found the hidden function!");
    std::println("This was executed via GDB 'call hidden_function()'\n");
    g_status = true;
}

int main() {
    int x = 10;
    int y = 5;

    std::println("Initial value of x: {}", x);
    
    if (x == 20) {
        std::println("SUCCESS: x was changed to 20 via GDB!");
    } else {
        std::println("x is still {}. Try changing it in GDB.", x);
    }

    std::println("This line might be skipped if you use 'jump'...");
    
    std::println("Reached the post-jump location.");

    int multiplication_result = multiply(x, y);
    std::println("Multiplication result: {}", multiplication_result);
    if (multiplication_result == 100) {
        std::println("SUCCESS: You forced the function to return 100!");
    }

    if (!g_status) {
        std::println("Graceful shutdown!");
    } else {
        std::println("Emergency shutdown!");
    }

    return 0;
}
