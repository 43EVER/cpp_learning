#include <iostream>
#include <algorithm>
#include <mutex>
#include <future>

int main()
{
    std::promise<int> ps;
    auto ft = ps.get_future();
    ps.set_value(42);
    std::cout << ft.get() << std::endl;
}