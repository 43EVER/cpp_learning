#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <sstream>
#include <shared_mutex>
#include <chrono>
#include <cstdlib>
#include <string>

using namespace std::chrono_literals;

struct Node {
    std::string msg;
    Node(const char* str) : msg(str) {}
};

Node operator "" _fuck(const char* str, std::size_t sz) {
    std::cout << sz << std::endl;
    return Node(str);
}

int main()
{
    std::thread t([]{
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "fuck" << std::endl;
        }
    });
    t.join();
}