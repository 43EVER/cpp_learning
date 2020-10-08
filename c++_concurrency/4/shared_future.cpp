#include <iostream>
#include <thread>
#include <future>
#include <cassert>
#include <array>
#include <vector>

int main()
{
    std::promise<int> ps;
    auto sf = ps.get_future().share();

    std::vector<std::thread> threads;
    for (int i = 0; i < 16; i++)
        threads.emplace_back([sf]{
            std::cout << std::this_thread::get_id() << " get " << sf.get() << std::endl;
        });
    
    ps.set_value(100);

    for (auto&& entry : threads)
        entry.join();
}