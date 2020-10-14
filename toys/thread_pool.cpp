#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <queue>
#include <future>
#include <atomic>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <iostream>

namespace std {

class threadpool {
    using Task = function<void()>;
    vector<thread> _pool;
    queue<Task> _tasks;
    mutex _lock;
    condition_variable _task_cv;
    atomic<bool> _run{ true };
    atomic<int> _idThrNum{ 0 };

public:
    inline threadpool(unsigned int size = min(thread::hardware_concurrency(), 4u)) { addThread(size); }
    inline ~threadpool()
    {
        _run = false;
        _task_cv.notify_all();
        for (thread& thread : _pool) {
            if (thread.joinable())
                thread.join();
        }
    }

public:
    template<typename F, typename... Args>
    auto commit(F&& f, Args&&... args)
    {
        if (!_run)
            throw runtime_error("commit on thread is stopped");
        
        using RetType = decltype(f(args...));
        auto task = make_shared<packaged_task<RetType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<RetType> future = task->get_future();
        {
            scoped_lock lock{ _lock };
            _tasks.emplace([task]
            {
                (*task)();
            });
        }

        _task_cv.notify_one();

        return future;
    }

    int idCount() { return _idThrNum; }
    int thrCount() { return _pool.size(); }
    
private:
    void addThread(unsigned int size)
    {
        for (; size > 0; --size) {
            _pool.emplace_back([this]{
                while (_run) {
                    Task task;
                    {
                        unique_lock<mutex> lk{ _lock };
                        _task_cv.wait(lk, [this]{
                            return !_run || !_tasks.empty();
                        });
                        if (!_run && _tasks.empty())
                            return;

                        task = move(_tasks.front());
                        _tasks.pop();
                    }
                    _idThrNum--;
                    task();
                    _idThrNum++;
                }
            });
            _idThrNum++;
        }
    }
};


}

void fun(const std::string& str)
{
    for (int i = 0; i < 1e9; i++);
    std::cout << str << std::endl;
}

int main()
{
    std::vector<std::future<void>> vec;

    std::threadpool t(16);
    for (int i = 0; i < 100; i++)
        vec.push_back(t.commit(fun, std::string("id") + std::to_string(i)));

    for (auto&& entry : vec)
        entry.get();
}


#endif