#include <iostream>
#include <vector>

void Swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void BubbleSort(std::vector<int>& arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; ++j)
        {
            // A good place to set a breakpoint and use TUI
            if (arr[j] > arr[j + 1])
            {
                Swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main()
{
    std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90};

    std::cout << "Original array:\n";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    BubbleSort(numbers);

    std::cout << "Sorted array:\n";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
