#include <chrono>
#include <mutex>
#include <print>
#include <pthread.h>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

std::mutex mutexA;
std::mutex mutexB;

void ThreadAFunc()
{
    pthread_setname_np(pthread_self(), "ThreadA");
    std::println("[Thread A] Attempting to lock Mutex A...");
    std::lock_guard<std::mutex> lockA(mutexA);
    std::println("[Thread A] Mutex A locked. Sleeping to ensure deadlock...");

    std::this_thread::sleep_for(2s);

    std::println("[Thread A] Attempting to lock Mutex B (this will block)...");
    std::lock_guard<std::mutex> lockB(mutexB);
    std::println("[Thread A] Mutex B locked. (You won't see this)");
}

void ThreadBFunc()
{
    pthread_setname_np(pthread_self(), "ThreadB");
    std::println("[Thread B] Attempting to lock Mutex B...");
    std::lock_guard<std::mutex> lockB(mutexB);
    std::println("[Thread B] Mutex B locked. Sleeping to ensure deadlock...");

    std::this_thread::sleep_for(2s);

    std::println("[Thread B] Attempting to lock Mutex A (this will block)...");
    std::lock_guard<std::mutex> lockA(mutexA);
    std::println("[Thread B] Mutex A locked. (You won't see this)");
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("Deadlock Demonstration (PID: {})", getpid());
    std::println("======================================");
    std::println("Thread A will lock Mutex A, then try Mutex B.");
    std::println("Thread B will lock Mutex B, then try Mutex A.");
    std::println("A deadlock is guaranteed to occur.");

    std::jthread t1(ThreadAFunc);
    std::jthread t2(ThreadBFunc);

    // Threads will join automatically on destruction of jthread,
    // but they will stay deadlocked forever.

    return 0;
}
