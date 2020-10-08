#include <iostream>
#include <algorithm>
#include <atomic>
#include <thread>
#include <shared_mutex>
#include <mutex>

std::vector<int> data;
std::atomic<bool> data_ready(false);

void read_thread()
{
    while (!data_ready.load()) // 1：happens-before 2
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << data[0]; // 2
}

void write_thread()
{
    data.push_back(42); // 3：happens-before 4
    data_ready = true; // 4：inter-thread happens-before 1
}

int main()
{
    std::thread t1(read_thread), t2(write_thread);
}