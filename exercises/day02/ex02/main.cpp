#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <print>

using namespace std::literals;

int main() {
    pid_t pid = getpid();
    int counter = 0;

    std::println("=== Attach to Process Exercise ===");
    std::println("My Process ID (PID) is: {}", pid);
    std::println("I'm going into a loop now. Try attaching GDB to me!");

    while (true) {
        counter++;
        std::println("Current counter: {} (PID: {})", counter, pid);
        std::this_thread::sleep_for(2s);
    }

    return 0;
}
