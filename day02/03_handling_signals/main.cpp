#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <print>
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
    std::println("Caught signal {} ({})", signum, strsignal(signum));

    if (signum == SIGSEGV || signum == SIGFPE || signum == SIGABRT)
    {
        std::println("Exiting due to fatal signal.");
        _exit(signum);
    }
}

static void TriggerSegfault()
{
    std::println("Triggering SIGSEGV...");
    volatile int* p = nullptr;
    *p = 42;
}

static void TriggerFPE()
{
    std::println("Triggering SIGFPE...");
    volatile int a = 42;
    volatile int b = 0;
    [[maybe_unused]] int c = a / b;
}

static void TriggerInt()
{
    std::println("Triggering SIGINT...");
    raise(SIGINT);
}

static void TriggerTerm()
{
    std::println("Triggering SIGTERM...");
    raise(SIGTERM);
}

static void TriggerAbort()
{
    std::println("Triggering SIGABRT...");
    abort();
}

static void TriggerUsr1()
{
    std::println("Triggering SIGUSR1...");
    raise(SIGUSR1);
}

static std::optional<Choice> GetUserChoice()
{
    std::println("\n--- Signal Handler Demo ---");
    std::println("1. Trigger SIGSEGV");
    std::println("2. Trigger SIGFPE");
    std::println("3. Trigger SIGINT");
    std::println("4. Trigger SIGTERM");
    std::println("5. Trigger SIGABRT");
    std::println("6. Trigger SIGUSR1");
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
