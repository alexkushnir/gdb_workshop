#pragma once
#include <string>
#include <vector>

struct HeavyData {
    std::string name;
    std::vector<double> metrics;
    HeavyData(std::string n) : name(n), metrics(100, 3.14) {}
};

class HeavyUtil {
    HeavyData data{"HeavyUtil"};
public:
    void heavy_compute() { /* Simulate bloat */ }
};
