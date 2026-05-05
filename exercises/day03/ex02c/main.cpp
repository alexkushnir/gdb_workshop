#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <list>
#include <latch>
#include <atomic>
#include <memory>

using TIME_UNIT = std::chrono::milliseconds;
// using TIME_UNIT = std::chrono::microseconds;
using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Task {
    const size_t index;
    const int time;
    const int max_time;
    boost::asio::io_context& ioContext;
    std::latch& work_done;

    std::atomic<bool> guard{false};
    int result = -2; // intentionally non-atomic

    static void timerHandler(const boost::system::error_code& ec,
                             Task& task,
                             time_point start) {
        if (ec == boost::asio::error::operation_aborted) {
            std::cout << "Timer for task " << task.index << " was canceled" << std::endl;
        } else if (!ec) {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);

            std::cout << "Timer for task " << task.index
                      << " expired after " << duration << std::endl;

            task.result = -1; // <-- unsynchronized write, now dangerous

            bool expected = false;
            bool success = task.guard.compare_exchange_strong(expected, true);
            if (!success) {
                std::cout << "Task " << task.index
                          << ", already wrote, before timer expiration" << std::endl;
            } else {
                task.work_done.count_down();
            }
        }
    }

public:
    Task(size_t index, int time, int max_time,
         boost::asio::io_context& ioContext,
         std::latch& work_done)
        : index(index), time(time), max_time(max_time),
          ioContext(ioContext), work_done(work_done) {}

    void submit(boost::asio::thread_pool& pool) {
        boost::asio::post(pool, [this] {
            boost::asio::steady_timer timer(ioContext, TIME_UNIT(max_time));
            auto start = std::chrono::system_clock::now();

            timer.async_wait([this, start](const boost::system::error_code& ec) {
                timerHandler(ec, *this, start);
            });

            std::this_thread::sleep_for(TIME_UNIT(time));

            timer.cancel();

            result = time; // <-- unsynchronized write, now dangerous

            bool expected = false;
            bool success = guard.compare_exchange_strong(expected, true);
            if (!success) {
                std::cout << "Task " << index
                          << ", timer already expired and wrote" << std::endl;
            } else {
                std::cout << "Task " << index
                          << ", finished and wrote after " << TIME_UNIT(time) << std::endl;
                work_done.count_down();
            }
        });
    }

    int get_result() const {
        return result;
    }
};

void run_tasks(std::vector<int> task_times,
               int max_time,
               boost::asio::io_context& ioContext,
               boost::asio::thread_pool& pool) {
    auto start = std::chrono::system_clock::now();

    const long num_tasks = static_cast<long>(task_times.size());
    std::latch work_done(num_tasks);

    std::list<Task> tasks;
    size_t task_index = 0;
    for (auto task_time : task_times) {
        tasks.emplace_back(task_index, task_time, max_time, ioContext, work_done);
        ++task_index;
    }

    task_index = 0;
    for (auto& task : tasks) {
        std::this_thread::sleep_for(TIME_UNIT(50));
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);

        std::cout << "Submitting task " << task_index
                  << " at time: " << duration << std::endl;
        ++task_index;

        task.submit(pool);
    }

    work_done.wait();

    std::cout << "Results:" << std::endl;
    task_index = 0;
    for (auto& task : tasks) {
        std::cout << " >>> Task " << task_index
                  << " (" << task_times[task_index] << "): "
                  << task.get_result() << std::endl;
        ++task_index;
    }
}

int main() {
    auto start = std::chrono::system_clock::now();

    boost::asio::io_context ioContext;
    auto workGuard = boost::asio::make_work_guard(ioContext);

    std::jthread ioThread([&ioContext]() {
        ioContext.run();
    });

    constexpr std::size_t pool_size = 3;
    boost::asio::thread_pool pool(pool_size);

    std::vector task_times = {100, 500, 1800, 599, 700, 900, 300};
    constexpr int max_time = 600;

    run_tasks(std::move(task_times), max_time, ioContext, pool);

    pool.join();

    workGuard.reset();
    ioContext.stop();

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
    std::cout << "Total runtime: " << duration << std::endl;
}
