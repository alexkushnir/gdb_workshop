#include <print>
#include <memory>
#include <string>
#include <vector>

struct Manager
{
    std::string name;
    int experience;
};

struct Workplace
{
    std::string branchName;
    Manager* manager;
};

void PrintManagerDetails(const Workplace& shop)
{
    std::println("--- Workplace Details ---");
    std::println("Branch: {}", shop.branchName);

    // INTENTIONAL CRASH: Null pointer dereference
    // When analyzing the core dump, 'p shop.manager' will show 0x0.
    std::println("Manager Name: {}", shop.manager->name);
    std::println("Manager Experience: {} years", shop.manager->experience);
}

void ProcessWorkplace(const Workplace& shop, int depth)
{
    std::string frameInfo = "Processing depth " + std::to_string(depth);

    if (depth > 0)
    {
        ProcessWorkplace(shop, depth - 1);
    }
    else
    {
        PrintManagerDetails(shop);
    }
}

void StartProcessing(const std::string& area, int intensity)
{
    Workplace londonShop{"London Central", nullptr};

    std::vector<std::string> tags = {"Retail", "Flagship", "Fashion", "High-Priority"};

    std::println("Processing area: {} with intensity {}", area, intensity);

    ProcessWorkplace(londonShop, 3);
}

int main()
{
    std::string rootArea = "Greater London Zone 1";
    int initialIntensity = 42;

    std::println("Starting the application...");
    StartProcessing(rootArea, initialIntensity);

    return 0;
}