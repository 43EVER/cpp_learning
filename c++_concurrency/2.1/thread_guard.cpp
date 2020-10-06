#include <iostream>
#include <algorithm>
#include <thread>

class thread_guard {
private:
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard()
    {
        if (t.joinable())
            t.join();
    }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

struct func {
    int& i;
    func(int& i_) : i(i_) {}
    void operator() ()
    {
        for (unsigned j = 0; j < 1e6; ++j) {
            ++i;
        }
    }
};

void f()
{
    int local_state = 0;
    func my_func(local_state);
    std::thread t(my_func);
    thread_guard g(t);
    for (int i = 0; i < 1e9; i++);
}

int main()
{
    f();
}