#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Item
{
    int m_id;
    std::string m_name;
    double m_price;
    int m_quantity;
};

void ProcessOrder(Item& item)
{
    if (item.m_quantity > 0)
    {
        item.m_quantity--;
        std::cout << "Processed 1 " << item.m_name << ", " << item.m_quantity
                  << " remaining at $" << item.m_price << "\n";
    }
    else
    {
        std::cout << "Out of stock: " << item.m_name << "\n";
    }
}

double CalculateTotal(const std::vector<Item>& items)
{
    double total = 0.0;
    for (const auto& item : items)
    {
        total += item.m_price * item.m_quantity;
    }
    return total;
}

int main()
{
    std::vector<Item> inventory = {{101, "Widget", 19.99, 5},
                                   {102, "Gadget", 29.50, 2},
                                   {103, "Doohickey", 5.00, 10},
                                   {104, "Thingamabob", 100.00, 0}};

    std::cout << "Starting order processor...\n";

    for (auto& item : inventory)
    {
        ProcessOrder(item);
    }

    double totalValue = CalculateTotal(inventory);
    std::cout << "Total remaining inventory value: $"
              << std::setprecision(std::numeric_limits<double>::max_digits10) << totalValue << "\n";

    return 0;
}
