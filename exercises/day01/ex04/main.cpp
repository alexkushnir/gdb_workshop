#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Function that might throw an exception.
void RiskyOperation(int value)
{
    if (value < 0)
    {
        throw std::invalid_argument("Value cannot be negative: " + std::to_string(value));
    }
    if (value > 100)
    {
        throw std::out_of_range("Value is too large: " + std::to_string(value));
    }
    std::cout << "Operation successful with value: " << value << "\n";
}

// Function with nested try-catch.
void ProcessData(const std::vector<int>& data)
{
    for (int itm : data)
    {
        try
        {
            RiskyOperation(itm);
        }
        catch (const std::out_of_range& e)
        {
            std::cerr << "Caught a range error: " << e.what() << "\n";
            // Rethrowing to see multiple catchpoints in action.
            throw;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught a general error: " << e.what() << "\n";
        }
    }
}

int main()
{
    std::vector<int> data = {10, -5, 50, 150, 20};

    std::cout << "Starting data processing...\n";

    try
    {
        ProcessData(data);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Main caught an unhandled exception: " << e.what() << "\n";
    }

    std::cout << "Processing complete.\n";
    return 0;
}
