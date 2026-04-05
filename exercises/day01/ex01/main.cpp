#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Item {
    int id;
    std::string name;
    double price;
    int quantity;
};

void process_order(Item& item) {
    if (item.quantity > 0) {
        item.quantity--;
        std::println("Processed 1 {}, {} remaining at ${}", item.name, item.quantity, item.price);
    } else {
        std::println("Out of stock: {}", item.name);
    }
}

double calculate_total(const std::vector<Item>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.price * item.quantity;
    }
    return total;
}

int main() {
    std::vector<Item> inventory = {
        {101, "Widget", 19.99, 5},
        {102, "Gadget", 29.50, 2},
        {103, "Doohickey", 5.00, 10},
        {104, "Thingamabob", 100.00, 0}
    };

    std::println("Starting order processor...");

    for (auto& item : inventory) {
        process_order(item);
    }

    double total_value = calculate_total(inventory);
    std::println("Total remaining inventory value: ${}", total_value);

    return 0;
}
