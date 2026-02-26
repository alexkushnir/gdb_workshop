#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <print>
#include <scn/scan.h>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>

enum class Choice : int
{
    ThrowException = 1,
    CatchException,
    TriggerSegfault,
    TriggerFork,
    TriggerExec,
    Exit
};

static void TriggerThrow()
{
    std::println("Triggering throw...");
    throw std::runtime_error("This is a test exception");
}

static void TriggerCatch()
{
    std::println("Triggering catch...");
    try
    {
        TriggerThrow();
    }
    catch (const std::exception& e)
    {
        std::println("Caught exception: {}", e.what());
    }
}

static void TriggerSegfault()
{
    std::println("Triggering SIGSEGV...");
    volatile int* p = nullptr;
    *p = 42;
}

static void TriggerFork()
{
    std::println("Triggering fork...");
    pid_t pid = fork();
    if (pid == 0)
    {
        std::println("Child process (PID: {})", getpid());
        _exit(0);
    }
    else if (pid > 0)
    {
        std::println("Parent process (PID: {}), created child {}", getpid(), pid);
        wait(nullptr);
    }
    else
    {
        std::println(stderr, "Fork failed!");
    }
}

static void TriggerExec()
{
    std::println("Triggering exec...");
    execl("/bin/ls", "ls", "-l", NULL);
    perror("execl failed");
}

static std::optional<Choice> GetUserChoice()
{
    std::println("\n--- GDB Catchpoints Demo ---");
    std::println("1. Throw Exception");
    std::println("2. Catch Exception");
    std::println("3. Trigger SIGSEGV");
    std::println("4. Trigger Fork");
    std::println("5. Trigger Exec");
    std::println("0. Exit");

    if (auto result = scn::prompt<int>("Select an option: ", "{}"))
    {
        auto [res] = result->values();
        return std::make_optional(Choice{res});
    }
    else
    {
        std::println(stderr, "Error: {}", result.error().msg());
        return std::nullopt;
    }
}

int main()
{
    bool exit = false;
    std::optional<Choice> choice;
    static const std::map<Choice, std::function<void()>> handlers = {
        {Choice::ThrowException, TriggerThrow},
        {Choice::CatchException, TriggerCatch},
        {Choice::TriggerSegfault, TriggerSegfault},
        {Choice::TriggerFork, TriggerFork},
        {Choice::TriggerExec, TriggerExec},
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
