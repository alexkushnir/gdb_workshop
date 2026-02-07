#include <vector>
#include <print>

struct Data 
{
    int id;
    int value;
};

void process_data(std::vector<Data>& data) 
{
    for (size_t i = 0; i < data.size(); ++i) 
    {
        data[i].value += 1;
        
        if (i == 10) 
        {
            std::println("Processing index 10... (and secretly corrupting index 42)");
            data[42].value = -9999; 
        }
    }
}

int main() 
{
    std::vector<Data> dataset(100);
    for (int i = 0; i < 100; ++i) 
    {
        dataset[i] = { .id = i, .value = i * 10};
    }

    process_data(dataset);

    if (dataset[42].value == (42 * 10) + 1) 
    {
        std::println("Verification successful!");
    } 
    else 
    {
        std::println("ERROR: Value at index 42 is corrupted! Expected {}, got {}", 
                     (42 * 10) + 1, dataset[42].value);
    }

    return 0;
}
