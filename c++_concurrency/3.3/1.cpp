#include <iostream>
#include <algorithm>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>

struct A {
    A() { std::cout << this << ": default constructor" << std::endl; }
    A(const A&) { std::cout << this << ": copy constructor" << std::endl; }
    ~A() { std::cout << this << ": destructor" << std::endl;}
    void doSomething()
    {
        std::cout << this << ": do something" << std::endl;
    }
};

std::once_flag flag;

std::shared_ptr<A> P;

void init()
{
    P.reset(new A);
}

void f()
{
    std::call_once(flag, init);
    P->doSomething();
}

int main()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
        threads.emplace_back(f);
    }

    for (auto&& entry : threads)
        entry.join();
}