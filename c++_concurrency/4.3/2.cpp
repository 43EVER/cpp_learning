#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop()
{
    auto const timeout = std::chrono::steady_clock::now() +
                            std::chrono::milliseconds(500);
    std::unique_lock lk(m);
    int i = 0;
    while (!done) {
        if (cv.wait_until(lk, timeout, []{ return done; }) == true)
            break;
        std::cout << i++ << std::endl;
    }
    return done;
}

int main()
{
    auto a = std::async(wait_loop);
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1600));
        std::unique_lock lk(m);
        done = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1600));
    }
    std::cout << a.get() << std::endl;
}