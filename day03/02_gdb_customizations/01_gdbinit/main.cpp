#include <iostream>
#include <string>
#include <vector>

struct DataPoint
{
    int m_id;
    std::string m_label;
};

void ProcessData(const std::vector<DataPoint>& points)
{
    std::cout << "Processing " << points.size() << " data points...\n";
    for (const auto& p : points)
    {
        std::cout << "  Point " << p.m_id << ": " << p.m_label << "\n";
    }
}

int main()
{
    std::cout << "GDB .gdbinit Customization Demo\n";
    std::cout << "===============================\n";

    std::vector<DataPoint> data = {{101, "Alpha"}, {202, "Beta"}, {303, "Gamma"}};

    ProcessData(data);

    std::cout << "Done.\n";
    return 0;
}
