#include <iostream>
#include <mutex>
#include <thread>
#include <stack>

std::stack<int> s;
std::mutex m;

int main()
{
    std::scoped_lock l(m);

    std::unique_lock l(m);
}