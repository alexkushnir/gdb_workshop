#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct DataPoint
{
    int m_id;
    double m_value;
    long m_timestamp;
};

void ProcessData(const DataPoint& dp)
{
    // Break or trace here
    double processed = std::sqrt(std::abs(dp.m_value)) * 2.0;
    std::cout << std::fixed << std::setprecision(2)
              << "Processing ID: " << dp.m_id << ", Raw: " << dp.m_value
              << ", Processed: " << processed << "\n"
              << std::defaultfloat;
}

int main()
{
    std::cout << "--- GDB Tracepoints Demo ---\n";
    std::cout << "This program generates data points in a loop.\n";
    std::cout << "Tracepoints allow collecting data without stopping the program.\n";
    std::cout << "----------------------------\n\n";

    std::vector<DataPoint> repository;

    for (int i = 0; i < 20; ++i)
    {
        DataPoint dp = {i,
                        std::sin(i * 0.1) * 100.0,
                        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

        ProcessData(dp);

        repository.push_back(dp);

        // Slow down a bit to make it easier to follow
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Demo finished.\n";
    return 0;
}
