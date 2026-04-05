#include <iostream>
#include <string>
#include <print>

// Function that will cause a crash.
void crash_here(int* ptr) {
    std::println("Attempting to dereference pointer...");
    *ptr = 42; // This will crash if ptr is null
}

void middle_function(int* ptr) {
    crash_here(ptr);
}

int main() {
    std::println("Crashing program starting...");
    int* my_ptr = nullptr;
    middle_function(my_ptr);
    return 0;
}
