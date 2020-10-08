#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>

class spinlock_mutex {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};

spinlock_mutex m;

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 16; i++)
    {
        threads.emplace_back([&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::scoped_lock l(m);
            std::cout << std::this_thread::get_id() << std::endl;
        });
    }

    for (auto&& entry : threads)
        entry.join();
}