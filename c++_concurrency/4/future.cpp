#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <cstdlib>

void delay()
{
    int t = (rand() % 100) * 1e5;
    while (t--);
}



int main()
{
    auto answer = std::async([]{
        for (int i = 0; i < 10; i++)
            std::cout << std::this_thread::get_id() << ": is running" << std::endl;
        return 100;
    });

    delay();
    std::cout << "doint other stuff" << std::endl;

    std::cout << answer.get() << std::endl;
}