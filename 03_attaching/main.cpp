#include <print>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void PrintFunction() {
    std::uint64_t counter = 0;
    while (true) {
        std::println("{}: {}", std::this_thread::get_id(), ++counter);
        std::this_thread::sleep_for(1s);
    }
}

void PrintFunctioWrapper() {
    PrintFunction();
}

void PrintFunctionWrapperWrapper() {
    PrintFunctioWrapper();
}

int main() {
    volatile bool stop = false;

    PrintFunctionWrapperWrapper();

    return 0;
}