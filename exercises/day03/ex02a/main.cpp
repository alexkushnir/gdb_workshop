#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <chrono>
#include <vector>
#include <list>
#include <latch>

using TIME_UNIT = std::chrono::milliseconds;
// using TIME_UNIT = std::chrono::microseconds;
using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Task {
    const size_t index;
    const int time;
    const int max_time;
    boost::asio::io_context& ioContext;
    std::latch& work_done;
    // internal managed vars:
    std::atomic<bool> guard;
    std::jthread my_thread;
    // task that doesn't finish on time shall got -1
    // otherwise, shall get the time in took to finish
    int result;

    // private function for the timer handler
    static void timerHandler(const boost::system::error_code& ec, Task& task, time_point start, pthread_t thread_handler) {
        if (ec == boost::asio::error::operation_aborted) {
            std::cout << "Timer for task " << task.index << " was canceled" << std::endl;
        } else if (!ec) {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
            // print duration that passed
            std::cout << "Timer for task " << task.index << " expired after " << duration << std::endl;
            bool expected = false;
            bool success = task.guard.compare_exchange_strong(expected, true);
            if(!success) {
                // the task already wrote
                std::cout << "Task " << task.index << ", already wrote, before timer expiration" << std::endl;
            }
            else {
                task.result = -1;
                // send a cancel request to the thread that seems to be stuck
                pthread_cancel(thread_handler);
                task.work_done.count_down();
            }
        }
    }
public:
    Task(size_t index, int time, int max_time, boost::asio::io_context& ioContext, std::latch& work_done)
        : index(index), time(time), max_time(max_time), ioContext(ioContext), work_done(work_done) {}
    void run() {
        my_thread = std::jthread([this] {
            // allow this thread to be canceled asyncronously from another thread
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
            pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);            
            // set the timer
            boost::asio::steady_timer timer(ioContext, TIME_UNIT(max_time));
            auto curr_time = std::chrono::system_clock::now();
            auto thread_handler = pthread_self();
            timer.async_wait([&](const boost::system::error_code& ec) {
                timerHandler(ec, *this, curr_time, thread_handler);
            });

            // this is the actual (dummy) task to run:
            std::this_thread::sleep_for(TIME_UNIT(time));

            // now we are after the task was done, or there was a timeout
            timer.cancel();
            // there still might be a race condition, this is why we have the atomic bool
            bool expected = false;
            bool success = guard.compare_exchange_strong(expected, true);
            if(!success) {
                // the timer expired and wrote
                std::cout << "Task " << index << ", timer already expired and wrote" << std::endl;
            }
            else {
                std::cout << "Task " << index << ", finished and wrote after " << TIME_UNIT(time) << std::endl;
                result = time;
                work_done.count_down();
            }
        });
    }
    int get_result() const {
        return result;
    }
    void join() {
        if(my_thread.joinable()) {
            my_thread.join();
        }
    }
    void detach() {
        if(my_thread.joinable()) {
            my_thread.detach();
        }
    }
};

// note: this can go into a TaskManager class
void run_tasks(std::vector<int> task_times, int max_time, boost::asio::io_context& ioContext) {
    using namespace std::chrono_literals;

    auto start = std::chrono::system_clock::now();

    const long num_tasks = long(task_times.size());
    std::latch work_done(num_tasks);

    // initiate tasks
    std::list<Task> tasks;
    size_t task_index = 0;
    for(auto task_time: task_times) {
        // index, time, max_time, boost::asio::io_context& ioContext, latch& work_done
        tasks.emplace_back(task_index, task_time, max_time, ioContext, work_done);
        ++task_index;
    }

    task_index = 0;
    for(auto& task: tasks) {
        std::this_thread::sleep_for(TIME_UNIT(50));
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
        std::cout << "Starting task " << task_index << " at time: " << duration << std::endl;
        ++task_index;
        task.run();
    }

    // wait on latch, for all threads to report done
    work_done.wait();

    std::cout << "Results:" << std::endl;
    task_index = 0;
    for(auto& task: tasks) {
        std::cout << " >>> Task " << task_index << " (" << task_times[task_index] << "): " << task.get_result() << std::endl;
        ++task_index;
        // detaching the threads is not mandatory, as we cancel stucked threads
        // but can serve as an extra measure for aborting stuck threads 
        task.detach();
    }
}

int main() {
    auto start = std::chrono::system_clock::now();
    boost::asio::io_context ioContext;
    
    // work guard to keep io_context running till stopped
    // even if there is no active timer to wait for
    auto workGuard = boost::asio::make_work_guard(ioContext);
    
    // Run io_context in a separate thread
    std::jthread ioThread([&ioContext]() {
        ioContext.run();
    });

    // tasks are simulating time, higher number = longer task
    std::vector task_times = {100, 500, 1800, 599, 700, 900, 300};
    constexpr int max_time = 600;
    run_tasks(std::move(task_times), max_time, ioContext);

    // stop the io_context
    ioContext.stop();

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
    // print duration that passed
    std::cout << "Total runtime: " << duration << std::endl;
}