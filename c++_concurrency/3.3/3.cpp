#include <iostream>
#include <mutex>
#include <thread>

std::recursive_mutex m;

void f ()
{
    std::scoped_lock l(m);
}

void g()
{
    std::scoped_lock l(m);
    f();
}

int main()
{
    std::thread t(g);
    t.join();
}