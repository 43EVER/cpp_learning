#include <iostream>
#include <algorithm>
#include <mutex>
#include <cctype>
#include <memory>

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
        check_for_hierachy_violation();
        internal_mutex.lock();
        update_hierachy_value();
    }

    void unlock()
    {
        if (this_thread_hierachy_value != hierachy_value)
            throw std::logic_error("mutex hierachy violated");

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

hierachical_mutex high(10000);
hierachical_mutex mid(6000);
hierachical_mutex low(5000);

void hf();
void mf();
void lf();

void lf()
{
    std::scoped_lock l(low);
    std::cout << "void lf()" << std::endl;
}

void hf()
{
    std::scoped_lock l(high);
    std::cout << "void hf()" << std::endl;
    lf();
    mf();
}

void mf()
{
    std::scoped_lock l(mid);
    std::cout << "void mf()" << std::endl;
    hf();
}

int main()
{
    hf();
}