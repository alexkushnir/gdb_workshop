#include <cstring>
#include <functional>
#include <map>
#include <optional>
#include <print>
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
    static std::string longStr(102, 'A'); // Create a long string of 200 'A's
    return longStr.c_str();
}

static void StackOverflow()
{
    auto str = GetStr();
    std::println("--- Triggering Stack Buffer Overflow ---");
    char buffer[100];
    std::println("Buffer address: {:p}", static_cast<void*>(buffer));
    // Intentional overflow
    std::strcpy(buffer, str);
    std::println("Result: {}", buffer);
}

static void HeapOverflow()
{
    std::println("--- Triggering Heap Buffer Overflow ---");
    int* array = new int[5];
    // Intentional overflow
    for (int i = 0; i <= 5; ++i)
    {
        array[i] = i * 10;
    }

    auto badIndex = 1205; // Accessing far beyond allocated size
    std::println("Array address: {:p}", static_cast<void*>(array + badIndex));
    std::println("Result: {}", array[badIndex]); // Accessing far beyond allocated size
    delete[] array;
}

static void UseAfterFree()
{
    std::println("--- Triggering Use-After-Free ---");
    int* ptr = new int(42);
    delete ptr;
    // Intentional use-after-free
    std::println("Dereferencing freed pointer: {}", *ptr);
}

static void MemoryLeak()
{
    std::println("--- Triggering Memory Leak ---");
    int* leak = new int[100];
    for (int i = 0; i < 100; ++i)
    {
        leak[i] = i;
    }
    // 'leak' is never deleted
    std::println("Memory leaked.");
}

static std::optional<Choice> GetUserChoice()
{
    std::println("\n--- AddressSanitizer Demo ---");
    std::println("1. Trigger Stack Buffer Overflow");
    std::println("2. Trigger Heap Buffer Overflow");
    std::println("3. Trigger Use-After-Free");
    std::println("4. Trigger Memory Leak");
    std::println("0. Exit");

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
            std::println(stderr, "Error: Invalid input '{}'", input);
            return std::nullopt;
        }
    }
    else
    {
        std::println(stderr, "Error: {}", result.error().msg());
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
            std::println("Invalid option.");
            continue;
        }
        auto handler = handlers.find(choice.value());
        if (handler != handlers.end())
        {
            handler->second();
        }
        else
        {
            std::println("Unsupported option.");
        }
    }

    return 0;
}
