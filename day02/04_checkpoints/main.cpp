#include <chrono>
#include <print>
#include <string>
#include <thread>
#include <vector>

struct State
{
    int m_counter;
    std::string m_phase;
    std::vector<int> m_data;
};

void PrintState(const State& s)
{
    std::print(
        "State: [Counter: {}, Phase: '{}', Data size: {}] ", s.m_counter, s.m_phase, s.m_data.size());
    if (!s.m_data.empty())
    {
        std::print("Values: [");
        for (size_t i = 0; i < s.m_data.size(); ++i)
        {
            std::print("{}{}", s.m_data[i], (i == s.m_data.size() - 1 ? "" : ", "));
        }
        std::println("]");
    }
    else
    {
        std::println("");
    }
}

int main()
{
    State currentState = {0, "Initialization", {}};

    std::println("--- GDB Checkpoints Demo ---");
    std::println("PID: {}", (long)getpid());
    std::println("Instructions for GDB:");
    std::println("1. Set a breakpoint at a line where you want to save state.");
    std::println("2. Type 'checkpoint' to save the current process state.");
    std::println("3. Let the program run and change its state.");
    std::println("4. Type 'info checkpoints' to see saved states.");
    std::println("5. Type 'restart <id>' to roll back to a checkpoint.");
    std::println("----------------------------\n");

    while (currentState.m_counter < 10)
    {
        currentState.m_counter++;

        if (currentState.m_counter == 3)
        {
            currentState.m_phase = "Exploration";
        }
        else if (currentState.m_counter == 7)
        {
            currentState.m_phase = "Deep Analysis";
        }

        currentState.m_data.push_back(currentState.m_counter * 10);

        PrintState(currentState);

        // Break here to experiment with checkpoints
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::println("\nDemo completed. Final Counter: {}", currentState.m_counter);
    return 0;
}
