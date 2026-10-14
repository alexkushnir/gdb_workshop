#include <cstring>
#include <functional>
#include <map>
#include <optional>
#include <iostream>
#include <scn/scan.h>
#include <string>
#include <vector>

enum class Choice : int
{
    TriggerStackOverflow = 1,
    TriggerHeapOverflow,
    TriggerUseAfterFree,
    TriggerMemoryLeak,
    Exit = 0
};

const char* GetStr()
{
    static std::string longStr(102, 'A'); // Create a long string of 102 'A's
    return longStr.c_str();
}

static void StackOverflow()
{
    auto str = GetStr();
    std::cout << "--- Triggering Stack Buffer Overflow ---\n";
    char buffer[100];
    std::cout << "Buffer address: " << static_cast<const void*>(static_cast<void*>(buffer)) << "\n";
    // Intentional overflow
    std::strcpy(buffer, str);
    std::cout << "Result: " << buffer << "\n";
}

static void HeapOverflow()
{
    std::cout << "--- Triggering Heap Buffer Overflow ---\n";
    int* array = new int[5];

    // Intentional overflow
    for (int i = 0; i <= 5; ++i)
    {
        array[i] = i * 10;
    }

    auto badIndex = 1205; // Accessing far beyond allocated size
    std::cout << "Array address: " << static_cast<const void*>(static_cast<void*>(array + badIndex)) << "\n";
    std::cout << "Result: " << array[badIndex] << "\n"; // Accessing far beyond allocated size
    delete[] array;
}

static void UseAfterFree()
{
    std::cout << "--- Triggering Use-After-Free ---\n";
    int* ptr = new int(42);
    delete ptr;
    // Intentional use-after-free
    std::cout << "Dereferencing freed pointer: " << *ptr << "\n";
}

static void MemoryLeak()
{
    std::cout << "--- Triggering Memory Leak ---\n";
    int* leak = new int[100];
    for (int i = 0; i < 100; ++i)
    {
        leak[i] = i;
    }
    // 'leak' is never deleted
    std::cout << "Memory leaked.\n";
}

static std::optional<Choice> GetUserChoice()
{
    std::cout << "\n--- AddressSanitizer Demo ---\n";
    std::cout << "1. Trigger Stack Buffer Overflow\n";
    std::cout << "2. Trigger Heap Buffer Overflow\n";
    std::cout << "3. Trigger Use-After-Free\n";
    std::cout << "4. Trigger Memory Leak\n";
    std::cout << "0. Exit\n";

    if (auto result = scn::prompt<std::string>("Select an option: ", "{}"))
    {
        auto [input] = result->values();
        if (auto scan_result = scn::scan<int>(input, "{}"))
        {
            auto [choice_val] = scan_result->values();
            return std::make_optional(Choice{choice_val});
        }
        else
        {
            std::cerr << "Error: Invalid input '" << input << "'\n";
            return std::nullopt;
        }
    }
    else
    {
        std::cerr << "Error: " << result.error().msg() << "\n";
        return std::nullopt;
    }
}

int main()
{
    // Disable buffering for stdout so we see output before ASan aborts the process
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    bool shouldExit = false;
    std::optional<Choice> choice;
    static const std::map<Choice, std::function<void()>> handlers = {
        {Choice::TriggerStackOverflow, StackOverflow},
        {Choice::TriggerHeapOverflow, HeapOverflow},
        {Choice::TriggerUseAfterFree, UseAfterFree},
        {Choice::TriggerMemoryLeak, MemoryLeak},
        {Choice::Exit, [&shouldExit]() { shouldExit = true; }}};

    while (!shouldExit)
    {
        choice = GetUserChoice();
        if (!choice.has_value())
        {
            std::cout << "Invalid option.\n";
            continue;
        }
        auto handler = handlers.find(choice.value());
        if (handler != handlers.end())
        {
            handler->second();
        }
        else
        {
            std::cout << "Unsupported option.\n";
        }
    }

    return 0;
}
