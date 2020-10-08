#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <deque>

void delay()
{
    std::cout << std::this_thread::get_id() << " is running" << std::endl;
    for (int i = 0; i < 1e5; i++);
}

int main()
{
    std::packaged_task<int()> pt([&]{
        std::cout << "packaged task start" << std::endl;
        delay();
        return 100;
    });
    auto ft = pt.get_future();
    std::cout << "main thread is running" << std::endl;
    pt();
    std::cout << ft.get() << std::endl;
}