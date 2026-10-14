#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <iostream>
#include <scn/scan.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unistd.h>

enum class Choice : int
{
    TriggerSegfault = 1,
    TriggerFPE,
    TriggerInt,
    TriggerTerm,
    TriggerAbort,
    TriggerUsr1,
    Exit = 0
};

void SignalHandler(int signum)
{
    std::cout << "Caught signal " << signum << " (" << strsignal(signum) << ")\n";

    if (signum == SIGSEGV || signum == SIGFPE || signum == SIGABRT)
    {
        std::cout << "Exiting due to fatal signal.\n";
        _exit(signum);
    }
}

static void TriggerSegfault()
{
    std::cout << "Triggering SIGSEGV...\n";
    volatile int* p = nullptr;
    *p = 42;
}

static void TriggerFPE()
{
    std::cout << "Triggering SIGFPE...\n";
    volatile int a = 42;
    volatile int b = 0;
    [[maybe_unused]] int c = a / b;
}

static void TriggerInt()
{
    std::cout << "Triggering SIGINT...\n";
    raise(SIGINT);
}

static void TriggerTerm()
{
    std::cout << "Triggering SIGTERM...\n";
    raise(SIGTERM);
}

static void TriggerAbort()
{
    std::cout << "Triggering SIGABRT...\n";
    abort();
}

static void TriggerUsr1()
{
    std::cout << "Triggering SIGUSR1...\n";
    raise(SIGUSR1);
}

static std::optional<Choice> GetUserChoice()
{
    std::cout << "\n--- Signal Handler Demo ---\n";
    std::cout << "1. Trigger SIGSEGV\n";
    std::cout << "2. Trigger SIGFPE\n";
    std::cout << "3. Trigger SIGINT\n";
    std::cout << "4. Trigger SIGTERM\n";
    std::cout << "5. Trigger SIGABRT\n";
    std::cout << "6. Trigger SIGUSR1\n";
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
    // Register signal handlers
    std::signal(SIGSEGV, SignalHandler);
    std::signal(SIGFPE, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGUSR1, SignalHandler);
    std::signal(SIGUSR2, SignalHandler);
    // Disable buffering for stdout so we see output before _exit in signal handlers
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    bool exit = false;
    std::optional<Choice> choice;
    static const std::map<Choice, std::function<void()>> handlers = {
        {Choice::TriggerSegfault, TriggerSegfault},
        {Choice::TriggerFPE, TriggerFPE},
        {Choice::TriggerInt, TriggerInt},
        {Choice::TriggerTerm, TriggerTerm},
        {Choice::TriggerAbort, TriggerAbort},
        {Choice::TriggerUsr1, TriggerUsr1},
        {Choice::Exit, [&exit]() { exit = true; }}};

    while (!exit)
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
