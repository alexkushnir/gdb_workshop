#include <iostream>
#include <string>
#include <vector>
#include <print>

struct DataPoint {
    int id;
    std::string label;
};

void ProcessData(const std::vector<DataPoint>& points) {
    std::println("Processing {} data points...", points.size());
    for (const auto& p : points) {
        std::println("  Point {}: {}", p.id, p.label);
    }
}

int main() {
    std::println("GDB .gdbinit Customization Demo");
    std::println("===============================");

    std::vector<DataPoint> data = {
        {101, "Alpha"},
        {202, "Beta"},
        {303, "Gamma"}
    };

    ProcessData(data);

    std::println("Done.");
    return 0;
}
