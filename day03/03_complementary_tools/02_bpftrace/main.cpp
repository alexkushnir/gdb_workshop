#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

void SimulationStep()
{
    // 1. Try reading a non-existent config file
    std::cout << "[I/O] Attempting to read config.json...\n";
    std::ifstream config("config.json");
    if (!config.is_open())
    {
        std::cout << "[I/O] Warning: config.json not found (expected ENOENT).\n";
    }

    // 2. Try reading a fallback file in /tmp
    std::cout << "[I/O] Checking fallback: /tmp/app_fallback.conf...\n";
    std::ifstream fallback("/tmp/app_fallback.conf");

    // 3. Heartbeat log
    std::cout << "[I/O] Writing to app.log...\n";
    std::ofstream log("app.log", std::ios::app);
    if (log.is_open())
    {
        log << "Heartbeat: "
            << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << "\n";
    }

    // 4. Temp file dance
    std::cout << "[I/O] Creating and deleting temp file...\n";
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

    std::cout << "BPFtrace for Linux Demo Simulator (PID: " << getpid() << ")\n";
    std::cout << "==========================================\n";
    std::cout << "This simulator performs varied I/O activity every 5 seconds:\n";
    std::cout << "- Reads non-existent 'config.json'\n";
    std::cout << "- Checks '/tmp/app_fallback.conf'\n";
    std::cout << "- Appends to 'app.log'\n";
    std::cout << "- Creates and unlinks a temp file\n";
    std::cout << "==========================================\n";

    auto t = std::thread(
        []()
        {
            while (true)
            {
                std::cout << "\n[Sleep] From thread: Waiting 5 seconds...\n";
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        });
    while (true)
    {
        SimulationStep();
        std::cout << "\n[Sleep] Waiting 5 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
