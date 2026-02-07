#include <print>
#include <vector>

struct Data
{
    int id;
    int value;
};

void ProcessData(std::vector<Data>& data)
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
        dataset[i] = {.id = i, .value = i * 10};
    }

    ProcessData(dataset);

    return 0;
}
