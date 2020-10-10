#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class thread_safe_queue;

template<typename T>
class thread_safe_queue {
private:
    using q_typ = std::queue<T>;
    using self_typ = thread_safe_queue<T>;

    q_typ q;
    std::mutex m;
    std::condition_variable cv;

public:
    thread_safe_queue() {}
    thread_safe_queue(const self_typ& rhs)
    {
        std::scoped_lock l(rhs.m);
        q = rhs.q;
    }
    thread_safe_queue(self_typ&& rhs)
    {
        std::scoped_lock(rhs.m);
        q = std::move(rhs.q);
    }

    std::shared_ptr<T> pop()
    {
        std::scoped_lock l(m);
        auto res = std::make_shared<T>();
        if (q.size()) {
            res = std::make_shared<T>(q.front());
            q.pop();
        }
        return res;
    }

    std::shared_ptr<T> wait_pop()
    {
        std::unique_lock l(m);
        cv.wait(l, [this]{return q.size(); });
        auto res = std::make_shared<T>(q.front());
        q.pop();
        return res;
    }

    void push(const T& x)
    {
        std::scoped_lock l(m);
        q.push(x);
        cv.notify_one();
    }

    bool empty()
    {
        std::scoped_lock l(m);
        return q.empty();
    }
};

# endif