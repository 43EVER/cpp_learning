#include <map>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <string>
#include <vector>
#include <iostream>

class A {
private:
    mutable std::shared_mutex m;
    int n = 0;
public:
    void read()
    {
        std::shared_lock l(m);
        std::cout << std::this_thread::get_id() << ": read " << n << std::endl; 
    }

    void write()
    {
        std::unique_lock l(m);
        std::cout << std::this_thread::get_id() << ": write" << std::endl;
        ++n;
    }
};

int main()
{
    std::vector<std::thread> threads;

    A a;

    for (int i = 0; i < 16; i++)
    {
        if (i % 4 == 0)
            threads.emplace_back(&A::write, &a);
        else
            threads.emplace_back(&A::read, &a);
    }

    for (auto&& entry : threads)
        entry.join();
}