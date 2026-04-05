#include <chrono>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

void SimulationStep()
{
    // 1. Try reading a non-existent config file
    std::println("[I/O] Attempting to read config.json...");
    std::ifstream config("config.json");
    if (!config.is_open())
    {
        std::println("[I/O] Warning: config.json not found (expected ENOENT).");
    }

    // 2. Try reading a fallback file in /tmp
    std::println("[I/O] Checking fallback: /tmp/app_fallback.conf...");
    std::ifstream fallback("/tmp/app_fallback.conf");

    // 3. Heartbeat log
    std::println("[I/O] Writing to app.log...");
    std::ofstream log("app.log", std::ios::app);
    if (log.is_open())
    {
        log << "Heartbeat: "
            << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << "\n";
    }

    // 4. Temp file dance
    std::println("[I/O] Creating and deleting temp file...");
    std::string tempPath = "/tmp/bpftrace_demo_tmp.txt";
    std::ofstream tmp(tempPath);
    if (tmp.is_open())
    {
        tmp << "Transient data";
        tmp.close();
        unlink(tempPath.c_str());
    }
}

int main()
{
    // Disable buffering for stdout
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    std::println("BPFtrace for Linux Demo Simulator (PID: {})", getpid());
    std::println("==========================================");
    std::println("This simulator performs varied I/O activity every 5 seconds:");
    std::println("- Reads non-existent 'config.json'");
    std::println("- Checks '/tmp/app_fallback.conf'");
    std::println("- Appends to 'app.log'");
    std::println("- Creates and unlinks a temp file");
    std::println("==========================================");

    auto t = std::thread(
        []()
        {
            while (true)
            {
                std::println("\n[Sleep] From thread: Waiting 5 seconds...");
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        });
    while (true)
    {
        SimulationStep();
        std::println("\n[Sleep] Waiting 5 seconds...");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
