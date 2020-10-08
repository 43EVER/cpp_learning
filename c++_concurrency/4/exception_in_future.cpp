#include <future>
#include <iostream>
#include <thread>


int main()
{
    auto f = std::async([] {
        std::cout << std::this_thread::get_id() << " is running. " << std::endl;
        throw std::out_of_range("out of range");
    });
    for (unsigned i = 0; i < 1e6; ++i);
    std::cout << std::this_thread::get_id() << "(main) is running. " << std::endl;
    f.wait();
    f.get();
    std::cout << std::endl;
}