#include <iostream>
#include <string>

/**
 * Overloaded functions for GDB breakpoint demonstration.
 */

void print_message(int value) {
    std::cout << "[int] Message: " << value << std::endl;
}

void print_message(double value) {
    std::cout << "[double] Message: " << value << std::endl;
}

void print_message(const std::string& message) {
    std::cout << "[string] Message: " << message << std::endl;
}

namespace utils {
    void print_message(long value) {
        std::cout << "[long] Message from namespace utils: " << value << std::endl;
    }
}

int main() {
    std::cout << "GDB Overloaded Functions Demonstration" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    print_message(42);
    print_message(3.14159);
    print_message(std::string("Hello from GDB workshop!"));
    
    utils::print_message(123456789L);

    return 0;
}
