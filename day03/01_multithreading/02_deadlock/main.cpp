#include <chrono>
#include <mutex>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

std::mutex mutexA;
std::mutex mutexB;

void ThreadAFunc()
{
    pthread_setname_np(pthread_self(), "ThreadA");
    std::cout << "[Thread A] Attempting to lock Mutex A...\n";
    std::lock_guard<std::mutex> lockA(mutexA);
    std::cout << "[Thread A] Mutex A locked. Sleeping to ensure deadlock...\n";

    std::this_thread::sleep_for(2s);

    std::cout << "[Thread A] Attempting to lock Mutex B (this will block)...\n";
    std::lock_guard<std::mutex> lockB(mutexB);
    std::cout << "[Thread A] Mutex B locked. (You won't see this)\n";
}

void ThreadBFunc()
{
    pthread_setname_np(pthread_self(), "ThreadB");
    std::cout << "[Thread B] Attempting to lock Mutex B...\n";
    std::lock_guard<std::mutex> lockB(mutexB);
    std::cout << "[Thread B] Mutex B locked. Sleeping to ensure deadlock...\n";

    std::this_thread::sleep_for(2s);

    std::cout << "[Thread B] Attempting to lock Mutex A (this will block)...\n";
    std::lock_guard<std::mutex> lockA(mutexA);
    std::cout << "[Thread B] Mutex A locked. (You won't see this)\n";
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::cout << "Deadlock Demonstration (PID: " << getpid() << ")\n";
    std::cout << "======================================\n";
    std::cout << "Thread A will lock Mutex A, then try Mutex B.\n";
    std::cout << "Thread B will lock Mutex B, then try Mutex A.\n";
    std::cout << "A deadlock is guaranteed to occur.\n";

    std::thread t1(ThreadAFunc);
    std::thread t2(ThreadBFunc);

    // The joins will never return: the threads stay deadlocked forever.
    t1.join();
    t2.join();

    return 0;
}
