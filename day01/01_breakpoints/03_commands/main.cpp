#include <iostream>
#include <vector>
#include <string>

/**
 * A simple function we want to "spy" on using GDB commands.
 */
void process_data(int id, const std::string& info) {
    // In a real scenario, this might be a complex processing step.
    // We want to log the parameters without modifying the source code.
    std::cout << "DEBUG (app): Handling ID " << id << std::endl;
}

int main() {
    std::cout << "Breakpoint Commands Demonstration" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::vector<std::pair<int, std::string>> items = {
        {101, "Initializing sensor"},
        {102, "Reading data"},
        {201, "Calibrating"},
        {305, "Storing results"}
    };

    for (const auto& item : items) {
        process_data(item.first, item.second);
    }

    std::cout << "Done." << std::endl;
    return 0;
}
