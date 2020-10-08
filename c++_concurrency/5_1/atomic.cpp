#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

struct A { int a[1000]; };
struct B { int x, y; };

int main()
{
    std::cout << std::boolalpha 
        << std::atomic<A>{}.is_always_lock_free 
        << std::endl
        << std::atomic<B>{}.is_always_lock_free
        << std::endl;

}