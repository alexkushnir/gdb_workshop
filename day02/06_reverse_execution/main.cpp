#include <string>
#include <vector>
// TBD - test on docker

struct Data
{
    int m_id;
    std::string m_value;
};

void ProcessData(Data& data)
{
    if (data.m_id == 42)
    {
        // Imaginary complex logic that has a side effect
        data.m_value = "Corrupted";
    }
}

int main()
{
    std::vector<Data> dataset = {{1, "Initial"}, {42, "Important"}, {100, "Final"}};

    for (auto& item : dataset)
    {
        ProcessData(item);
    }

    // BREAKPOINT HERE: End of problematic flow.
    // Inspect 'dataset' to see that item 42 is corrupted.
    // Set a watchpoint on the memory and use 'reverse-continue' to find the cause.
    for (const auto& item : dataset)
    {
        if (item.m_value == "Corrupted")
        {
            break;
        }
    }

    return 0;
}
