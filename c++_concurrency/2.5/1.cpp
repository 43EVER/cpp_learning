#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

std::thread::id master_id;

void fun()
{
    int t = (rand() % 100) * 1e6;
    while (t--);
    std::cout << std::this_thread::get_id() << std::endl;
    if (std::this_thread::get_id() == master_id) {
        std::cout << "我是master" << std::endl;
    }
}

int main()
{
    std::vector<std::thread> vec;
    for (int i = 0; i < 20; i++)
        vec.emplace_back(fun);
    
    master_id = vec[rand() % 20].get_id();

    for (auto&& entry : vec)
        entry.join();
}