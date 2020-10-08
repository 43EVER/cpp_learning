#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <stack>

template<typename T>
class thread_safe_stack {
private:
    std::stack<T> s;
    mutable std::mutex m;

public:
    thread_safe_stack() {}
    thread_safe_stack(const thread_safe_stack& rhs)
    {
        std::scoped_lock l(rhs.m);
        s = rhs.s;
    }

    thread_safe_stack& operator=(const thread_safe_stack&) = delete;

    void push(const T& val)
    {
        std::scoped_lock l(m);
        s.push(std::move(val));
    }

    void pop(T& n)
    {
        std::scoped_lock l(m);
        n = s.top();
        s.pop();
    }

    bool empty() const
    {
        std::scoped_lock l(m);
        return s.empty();
    }
};