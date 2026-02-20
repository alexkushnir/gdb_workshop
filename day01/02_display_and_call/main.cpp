#include "AReallyImportantClass.h"
#include <print>
#include <thread>

using namespace std::literals;
std::stop_source stopSource;

static void ThreadStopFunc()
{
    stopSource.request_stop();
}

int main()
{
    AReallyImportantClass aric;
    std::println("Hello, World!");

    int a[10];

    for (auto& entry : a)
    {
        entry = 1;
    }

    auto t = std::thread(
        [](std::stop_token stopToken)
        {
            while (!stopToken.stop_requested())
            {
                std::println("Still looping...");
                std::this_thread::sleep_for(1s);
            }

            std::println("Stop was requested");
        },
        stopSource.get_token());

    t.join();

    return 0;
}
