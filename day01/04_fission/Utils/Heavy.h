#pragma once
#include <string>
#include <vector>

struct HeavyData
{
    std::string m_name;
    std::vector<double> m_metrics;
    HeavyData(std::string n) : m_name(n), m_metrics(100, 3.14) {}
};

class HeavyUtil
{
    HeavyData m_data{"HeavyUtil"};

public:
    void HeavyCompute()
    { /* Simulate bloat */
    }
};
