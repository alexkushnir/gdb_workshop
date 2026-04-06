#include <iostream>
#include <print>
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
        std::println(
            "Processed 1 {}, {} remaining at ${}", item.m_name, item.m_quantity, item.m_price);
    }
    else
    {
        std::println("Out of stock: {}", item.m_name);
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

    std::println("Starting order processor...");

    for (auto& item : inventory)
    {
        ProcessOrder(item);
    }

    double totalValue = CalculateTotal(inventory);
    std::println("Total remaining inventory value: ${}", totalValue);

    return 0;
}
