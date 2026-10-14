#include <csignal>
#include <functional>
#include <map>
#include <optional>
#include <iostream>
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
    std::cout << "Triggering throw...\n";
    throw std::runtime_error("This is a test exception");
}

static void TriggerCatch()
{
    std::cout << "Triggering catch...\n";
    try
    {
        TriggerThrow();
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}

static void TriggerSegfault()
{
    std::cout << "Triggering SIGSEGV...\n";
    volatile int* p = nullptr;
    *p = 42;
}

static void TriggerFork()
{
    std::cout << "Triggering fork...\n";
    pid_t pid = fork();
    if (pid == 0)
    {
        std::cout << "Child process (PID: " << getpid() << ")\n";
        _exit(0);
    }
    else if (pid > 0)
    {
        std::cout << "Parent process (PID: " << getpid() << "), created child " << pid << "\n";
        wait(nullptr);
    }
    else
    {
        std::cerr << "Fork failed!\n";
    }
}

static void TriggerExec()
{
    std::cout << "Triggering exec...\n";
    execl("/bin/ls", "ls", "-l", NULL);
    perror("execl failed");
}

static std::optional<Choice> GetUserChoice()
{
    std::cout << "\n--- GDB Catchpoints Demo ---\n";
    std::cout << "1. Throw Exception\n";
    std::cout << "2. Catch Exception\n";
    std::cout << "3. Trigger SIGSEGV\n";
    std::cout << "4. Trigger Fork\n";
    std::cout << "5. Trigger Exec\n";
    std::cout << "0. Exit\n";

    if (auto result = scn::prompt<int>("Select an option: ", "{}"))
    {
        auto [res] = result->values();
        return std::make_optional(Choice{res});
    }
    else
    {
        std::cerr << "Error: " << result.error().msg() << "\n";
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
