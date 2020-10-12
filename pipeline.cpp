#include <iostream>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <memory>

std::mutex osm;

namespace trival_pipeline {
struct Task {
    static int ID_COUNT;
    
    const int id;
    Task() : id(ID_COUNT++) {}

    void process1() const
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void process2() const
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void process3() const
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void process4() const
    {
        std::scoped_lock lk(osm);
        std::cout << std::this_thread::get_id() << ": " << id << " finish" << std::endl;
    }
};
int Task::ID_COUNT = 0;


std::queue<Task> q[4];
std::mutex m[4];
std::atomic<bool> isEnd[4];

void test()
{
    std::vector<Task> tasks(100);
    std::vector<std::thread> threads;
    for (auto&& entry : tasks)
        q[0].push(entry);

    auto fun1 = [&](int task_type)
    {
        while (!isEnd[task_type]) {
            std::unique_lock lk(m[task_type]);
            
            if (q[task_type].empty()) {
                if (task_type == 0) {
                    isEnd[0] = true;
                } else {
                    if (isEnd[task_type - 1] == true) isEnd[task_type] = true;
                }
                continue;
            }

            auto task = q[task_type].front();
            q[task_type].pop();
            lk.unlock();

            if (task_type < 3) {
                if (task_type == 0) std::bind(&Task::process1, &task)();
                else if (task_type == 1) std::bind(&Task::process2, &task)();
                else std::bind(&Task::process3, &task)();
                std::unique_lock lk2(m[task_type + 1]);
                q[task_type + 1].push(task);
                lk2.unlock();
            } else {
                std::bind(&Task::process4, &task)();
            }
        }
    };

    for (unsigned i = 0; i < 15; ++i) {
        threads.emplace_back(fun1, i % 4);
    }

    for (auto&& entry : threads)
        entry.join();
}

}

int main()
{
    std::function<void ()> fun1 = []()
    {
        int t = (rand() % 10) * 1e8;
        while (t--);
    };
    std::function<void ()> final = []()
    {
        std::scoped_lock lk(osm);
        std::cout << "finish: " << std::this_thread::get_id() << std::endl;
    };

    std::queue<std::function<void ()>> q;
    for (int i = 0; i < 4; i++) q.push(fun1);
    q.push(final);

    PipelineTask<std::function<void ()>> pipelinetask(q);
    Pipeline<decltype(pipelinetask), 5> pl(std::vector<decltype(pipelinetask)>(10, pipelinetask), 16);
}