#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <sstream>
#include <shared_mutex>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void f()
{
    std::unique_lock l(m);
    cv.wait(l, [] { return ready; });

    std::stringstream s;
    s << std::this_thread::get_id();
    data += std::string("\n") + s.str() + " process\n";
    
    processed = true;
    l.unlock();
    cv.notify_one();
}

int main()
{
    data = "data";
    std::thread t(f);
    {
        std::lock_guard l(m);
        
        data += std::string("\n") + "ready\n";
        ready = true;
        cv.notify_one();
    }
    {
        std::unique_lock l(m);
        cv.wait(l, [] { return processed; });
    }
    std::cout << data;
    t.join();
}