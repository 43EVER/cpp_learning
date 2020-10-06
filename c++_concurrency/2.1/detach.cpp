#include <iostream>
#include <algorithm>
#include <thread>
#include <cassert>
#include <cstdlib>

int main()
{
    std::thread t([]() {
        while (true) {
            for (int i = 0; i < 1e5; i++) std::cout << "|";
        }
    });
    t.detach();
    assert(!t.joinable());
}