#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

int cnt = 1;

void some_function(int id)
{
    int t = (rand() % 100) * 1e8;
    while (t--);
    std::cout << id << ": void some_function()" << std::endl;
}

std::thread f()
{
    return std::thread(some_function, cnt++);
}

std::thread g()
{
    std::thread t(some_function, cnt++);
    return t;
}

int main()
{
    std::cout << std::thread::hardware_concurrency() << std::endl;

    std::thread t1(some_function, cnt++);
    std::thread t2 = std::move(t1);
    t1 = std::thread(some_function, cnt++);
    std::thread t3;
    t3 = std::move(t2);
    // t1 = std::move(t3);

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();

    std::cout << std::endl;

    auto a = f();
    auto b = g();

    a.join();
    b.join();
    std::cout << std::endl;

    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 10; ++i) {
        threads.emplace_back(some_function, ++cnt);
    }

    for (auto&& entry : threads) {
        entry.join();
    }
}