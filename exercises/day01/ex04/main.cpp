#include <iostream>
#include <print>
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
    std::println("Operation successful with value: {}", value);
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
            std::println(stderr, "Caught a range error: {}", e.what());
            // Rethrowing to see multiple catchpoints in action.
            throw;
        }
        catch (const std::exception& e)
        {
            std::println(stderr, "Caught a general error: {}", e.what());
        }
    }
}

int main()
{
    std::vector<int> data = {10, -5, 50, 150, 20};

    std::println("Starting data processing...");

    try
    {
        ProcessData(data);
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Main caught an unhandled exception: {}", e.what());
    }

    std::println("Processing complete.");
    return 0;
}
