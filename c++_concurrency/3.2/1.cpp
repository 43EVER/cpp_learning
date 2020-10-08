#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>

std::mutex some_mutex;
std::list<int> some_list;

void add_to_list(int new_value)
{
    std::lock_guard guard(some_mutex);
    for (int i = 0; i < 1e9; i++);
    std::cout << std::this_thread::get_id() << " is running" << std::endl;
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

int main()
{
    std::vector<std::thread> vec;
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
        vec.emplace_back(add_to_list, i);
    }

    for (auto&& entry : vec)
        entry.join();
}