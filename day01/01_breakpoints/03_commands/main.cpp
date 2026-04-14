#include <iostream>
#include <string>
#include <vector>

/**
 * A simple function we want to "spy" on using GDB commands.
 */
void ProcessData(int id, const std::string& info)
{
    // In a real scenario, this might be a complex processing step.
    // We want to log the parameters without modifying the source code.
    std::cout << "DEBUG (app): Handling ID " << id << std::endl;
}

/**
 * A trigger function that will enable the breakpoint on ProcessData.
 */
void PrepareWork()
{
    std::cout << "DEBUG (app): System is ready for intensive logging." << std::endl;
}

int main()
{
    std::cout << "Breakpoint Commands Demonstration" << std::endl;
    std::cout << "---------------------------------" << std::endl;

    // This call should be ignored by GDB because Breakpoint 1 is initially disabled.
    ProcessData(0, "Pre-flight check");

    // This call should trigger Breakpoint 2, which will enable Breakpoint 1.
    PrepareWork();

    std::vector<std::pair<int, std::string>> items = {{101, "Initializing sensor"},
                                                      {102, "Reading data"},
                                                      {201, "Calibrating"},
                                                      {305, "Storing results"}};

    for (const auto& item : items)
    {
        ProcessData(item.first, item.second);
    }

    std::cout << "Done." << std::endl;
    return 0;
}
