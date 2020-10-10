#include <iostream>
#include "thread_safe_queue.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <functional>
#include <queue>

class Task {
private:
    static unsigned ID;
    int id;

    void process(unsigned sec)
    {
        std::this_thread::sleep_for(std::chrono::seconds(sec));
    }

public:
    Task() : id(ID++) {}
    Task(const Task& task) : id(task.id) {}
    
    void processA()
    {
        process(1);
    }

    void processB()
    {
        process(1);
    }

    void processC()
    {
        process(1);
    }

    void processD()
    {
        process(1);
        std::cout << id << std::endl;
    }
};

template<typename Task>
class Runner {
private:
    std::atomic<bool> isEnd;
    std::vector<std::thread> threads;
    thread_safe_queue<Task> tasks;
    std::condition_variable cv;
    std::mutex m;

public:
    Runner() : isEnd(false)
    {
        auto fun =  [this]
        {
            while (!isEnd) {
                std::unique_lock l(m);
                if (!tasks.empty()) {
                    auto task = tasks.pop();
                    l.unlock();
                    if (task) {                    
                        task->processA();
                        task->processB();
                        task->processC();
                        task->processD();
                    }
                } else {
                    l.unlock();
                }
                std::this_thread::yield();
            }
        };

        for (int i = 0; i < 4; i++)
            threads.emplace_back(fun);
    }

    void push(const Task& task)
    {
        tasks.push(task);
    }

    ~Runner()
    {
        while (!tasks.empty()) {
            std::this_thread::yield();
        }
        isEnd.store(true, std::memory_order_release);
        for (auto&& entry : threads)
            entry.join();
    }
};

unsigned Task::ID = 0;

class PipeLine {
private:
    std::vector<std::thread> threads;
    std::mutex m;
    std::queue<std::function<void()>> tasks;
    std::condition_variable cv;
    std::atomic<bool> isEnd;

public:
    PipeLine()
    {
        auto fun = [this]
        {
            while (!isEnd) {
                std::unique_lock l(m);
                if (tasks.size()) {
                    auto task = tasks.front();
                    tasks.pop();
                    l.unlock();
                    task();
                } else {
                    std::this_thread::yield();
                }
            }
        };
        for (unsigned i = 0; i < 15; i++)
            threads.emplace_back(fun);
    }

    void push(std::function<void()> task)
    {
        std::scoped_lock l(m);
        tasks.push(task);
    }

    ~PipeLine()
    {
        while (tasks.size()) {
            std::this_thread::yield();
        }
        isEnd.store(true, std::memory_order_release);
        for (auto&& entry : threads)
            entry.join();
    }
};

int main()
{
    
}