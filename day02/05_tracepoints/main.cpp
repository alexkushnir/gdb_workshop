#include <chrono>
#include <cmath>
#include <print>
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
    std::println(
        "Processing ID: {}, Raw: {:.2f}, Processed: {:.2f}", dp.m_id, dp.m_value, processed);
}

int main()
{
    std::println("--- GDB Tracepoints Demo ---");
    std::println("This program generates data points in a loop.");
    std::println("Tracepoints allow collecting data without stopping the program.");
    std::println("----------------------------\n");

    std::vector<DataPoint> repository;

    for (int i = 0; i < 100; ++i)
    {
        DataPoint dp = {i,
                        std::sin(i * 0.1) * 100.0,
                        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

        ProcessData(dp);

        repository.push_back(dp);

        // Slow down a bit to make it easier to follow
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::println("Demo finished.");
    return 0;
}
