#include <mutex>
#include <stack>
#include <shared_mutex>
#include <memory>

template<typename T>
class thread_safe_stack {
private:
    std::stack<T> s;
    std::mutex m;

public:
    thread_safe_stack() {};
    thread_safe_stack(const thread_safe_stack& s)
    {
        std::scoped_lock l(s.m);
        this->s = s.s;
    }
    thread_safe_stack(thread_safe_stack&& stk)
    {
        std::scoped_lock l(s.m);
        this->s = std::move(stk.s);
    }
    thread_safe_stack& operator=(const thread_safe_stack&) = delete;
    
    std::shared_ptr<T> pop()
    {
        std::scoped_lock l(m);
        auto res = std::make_shared(s.top());
        s.pop();
        return res;
    }

    bool empty()
    {
        std::scoped_lock l(m);
        return s.empty(); 
    }

    void push(const T& x)
    {
        std::scoped_lock l(m);
        s.push(x);
    }
};