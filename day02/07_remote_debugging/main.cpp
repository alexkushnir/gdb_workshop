#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    int counter = 0;
    std::cout << "Remote debugging demo starting..." << std::endl;
    std::cout << "Run this under gdbserver: gdbserver :1234 ./remote_debugging" << std::endl;

    while (true)
    {
        counter++;
        if (counter % 5 == 0)
        {
            std::cout << "Step: " << counter << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
