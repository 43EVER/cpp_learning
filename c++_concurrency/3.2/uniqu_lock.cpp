#include <iostream>
#include <algorithm>
#include <thread>
#include <cstdlib>
#include <mutex>
#include <vector>

class hierachical_mutex {
private:
    std::mutex internal_mutex;
    const unsigned long hierachy_value;
    unsigned long previous_hierachy_value;
    static thread_local unsigned long this_thread_hierachy_value;
    
    void check_for_hierachy_violation()
    {
        if (this_thread_hierachy_value <= hierachy_value)
            throw std::logic_error("mutex hierachy violated");
    }

    void update_hierachy_value()
    {
        previous_hierachy_value = this_thread_hierachy_value;
        this_thread_hierachy_value = hierachy_value;
    }

    void rollback_hierachy_value()
    {
        this_thread_hierachy_value = previous_hierachy_value;
    }

public:
    explicit hierachical_mutex(unsigned long value):
        hierachy_value(value), previous_hierachy_value(0)
    {}

    void lock()
    {
        std::cout << std::this_thread::get_id() << ": lock" << std::endl;
        check_for_hierachy_violation();
        internal_mutex.lock();
        update_hierachy_value();
    }

    void unlock()
    {
        if (this_thread_hierachy_value != hierachy_value)
            throw std::logic_error("mutex hierachy violated");

        std::cout << std::this_thread::get_id() << ": unlock" << std::endl;
        
        rollback_hierachy_value();
        internal_mutex.unlock();
    }

    bool try_lock()
    {
        check_for_hierachy_violation();
        if (!internal_mutex.try_lock()) return false;
        update_hierachy_value();
        return true;
    }
};

thread_local unsigned long
hierachical_mutex::this_thread_hierachy_value = 0x3f3f3f3f;

unsigned delay()
{
    auto start = clock();
    int t = (rand() % 100) * 1e7;
    while (t--);
    return (clock() - start) * 1000 / CLOCKS_PER_SEC;
}

hierachical_mutex some_mutex(6000);
std::unique_lock<hierachical_mutex> prepare_data(std::unique_lock<hierachical_mutex>&& lk);

std::unique_lock<hierachical_mutex> get_lock()
{
    std::unique_lock lk(some_mutex);
    return prepare_data(std::move(lk));
}

std::unique_lock<hierachical_mutex> prepare_data(std::unique_lock<hierachical_mutex>&& lk)
{
    auto time = delay();
    std::unique_lock lk2(std::move(lk));
    std::cout << "cost " << time << " mill seconds" << std::endl << __LINE__ << std::endl;
    std::cout << std::this_thread::get_id() << " is running prepare_data" << std::endl;
    return lk2;
}

void process_data()
{
    auto time = delay();
    std::unique_lock lk(get_lock());
    std::cout << "cost " << time << " mill seconds" << std::endl;
    std::cout << std::this_thread::get_id() << " is running process_data" << std::endl;
}

int main()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
        threads.emplace_back(process_data);
    }

    for (auto&& entry : threads)
        entry.join();
}