#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <unistd.h>

struct State
{
    int m_counter;
    std::string m_phase;
    std::vector<int> m_data;
};

void PrintState(const State& s)
{
    std::cout << "State: [Counter: " << s.m_counter << ", Phase: '" << s.m_phase
              << "', Data size: " << s.m_data.size() << "] ";
    if (!s.m_data.empty())
    {
        std::cout << "Values: [";
        for (size_t i = 0; i < s.m_data.size(); ++i)
        {
            std::cout << s.m_data[i] << (i == s.m_data.size() - 1 ? "" : ", ");
        }
        std::cout << "]\n";
    }
    else
    {
        std::cout << "\n";
    }
}

int main()
{
    State currentState = {0, "Initialization", {}};

    std::cout << "--- GDB Checkpoints Demo ---\n";
    std::cout << "PID: " << (long)getpid() << "\n";
    std::cout << "Instructions for GDB:\n";
    std::cout << "1. Set a breakpoint at a line where you want to save state.\n";
    std::cout << "2. Type 'checkpoint' to save the current process state.\n";
    std::cout << "3. Let the program run and change its state.\n";
    std::cout << "4. Type 'info checkpoints' to see saved states.\n";
    std::cout << "5. Type 'restart <id>' to roll back to a checkpoint.\n";
    std::cout << "----------------------------\n\n";

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

    std::cout << "\nDemo completed. Final Counter: " << currentState.m_counter << "\n";
    return 0;
}
