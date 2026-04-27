#include <atomic>
#include <chrono>
#include <print>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std::chrono_literals;

struct Vault
{
    int m_accessCode = 4277;
    std::string m_secretPayload = "GDB_IS_AWESOME_2026";
    bool m_isUnlocked = false;
};

Vault globalVault;
std::atomic<bool> shouldExit{false};

void DeepComputeLayer3(int id)
{
    // This provides a nice deep stack for backtrace practice
    std::println("[SYSTEM] Processing secure data fragment #{}...", id);
    std::this_thread::sleep_for(500ms);
}

void DeepComputeLayer2(int id)
{
    DeepComputeLayer3(id);
}
void DeepComputeLayer1(int id)
{
    DeepComputeLayer2(id);
}

void SecurityHeartbeat()
{
    static int beat = 0;
    if (globalVault.m_isUnlocked)
    {
        std::println("[SYSTEM] STATUS: !!! COMPROMISED !!! - Secret: {}", globalVault.m_secretPayload);
    }
    else
    {
        std::println("[SYSTEM] STATUS: SECURE - Heartbeat #{}", ++beat);
    }
}

void RunSecurityLayer3()
{
    SecurityHeartbeat();
}
void RunSecurityLayer2()
{
    RunSecurityLayer3();
}
void RunSecurityLayer1()
{
    RunSecurityLayer2();
}

int main()
{
    std::println("========================================");
    std::println("   SECURE DATA VAULT SYSTEM v2.0       ");
    std::println("========================================");
    std::println("Process ID: {}", getpid());
    std::println("System is running. Attach GDB to inspect.");
    std::println("----------------------------------------");

    int fragmentId = 0;
    while (!shouldExit)
    {
        SecurityHeartbeat();
        DeepComputeLayer1(++fragmentId);
        std::this_thread::sleep_for(1s);
    }

    return 0;
}