#include <memory>
#include <cstring>
#include <fstream>
#include <print>

struct DataPoint
{
    int m_id = 14;
    double m_value = 3.14;
    long m_timestamp = 1777490330;
};

int main()
{
    DataPoint dp;
    constexpr size_t size = sizeof(dp);
    auto buf = std::make_unique<char[]>(size);    

    auto dummy = new int[10];
    dummy[0] = 12345;

    std::memcpy(buf.get(), &dp, size);

    std::ofstream outFile("out.bin", std::ios::binary | std::ios::out);
    outFile.write(buf.get(), size);
    outFile.close();

    std::ifstream inFile("out.bin", std::ios::binary | std::ios::in);

    char buf2[size] = { 0 };
    inFile.read(buf2, size); 
    
    for (const auto& it : buf2)
    {
        std::print("{:02x} ", it);
    }
    
    std::println("");
    return 0;
}