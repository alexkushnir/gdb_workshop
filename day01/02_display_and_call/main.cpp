#include "AReallyImportantClass.h"
#include <atomic>
#include <iostream>
#include <thread>

using namespace std::literals;
std::atomic<bool> stopRequested{false};

static void ThreadStopFunc()
{
    stopRequested = true;
}

int main()
{
    AReallyImportantClass aric;
    std::cout << "Hello, World!\n";

    int a[10];

    for (auto& entry : a)
    {
        entry = 1;
    }

    auto t = std::thread(
        []()
        {
            while (!stopRequested)
            {
                std::cout << "Still looping...\n";
                std::this_thread::sleep_for(1s);
            }

            std::cout << "Stop was requested\n";
        });

    t.join();

    return 0;
}
