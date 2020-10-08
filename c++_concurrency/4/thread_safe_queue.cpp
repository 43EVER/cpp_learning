#include <iostream>
#include <condition_variable>
#include <thread>
#include <deque>
#include <mutex>
#include <vector>


template<typename T>
class thread_safe_queue {
private:
    mutable std::mutex m;
    std::condition_variable cv;
    std::deque<T> container;
public:
    thread_safe_queue() {};
    thread_safe_queue(const thread_safe_queue& q)
    {
        container = q.container;
    }
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;

    void push(T new_value)
    {
        std::lock_guard l(m);
        container.push_back(new_value);
        cv.notify_one();
    }

    bool try_pop(T& value)
    {
        std::scoped_lock l(m);
        if (container.empty())
            return false;
        value = container.front();
        container.pop_front();
        return true;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock l(m);
        cv.wait(l, [this]{ return !container.empty(); });
        value = container.front();
        container.pop_front();
    }

    bool empty() const
    {
        std::scoped_lock l(m);
        return container.empty();
    }
};

thread_safe_queue<int> q;

int main()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 16; ++i) {
        threads.emplace_back([&] {
            int v;
            q.wait_and_pop(v);
            for (unsigned i = 0; i < 1e8; i++);
            std::cout << v << std::endl;
        });
    }

    threads.emplace_back([&] {
        int i = 100;
        while (i--) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            q.push(i);
        }
    });

    for (auto&& entry : threads)
        entry.join();
}