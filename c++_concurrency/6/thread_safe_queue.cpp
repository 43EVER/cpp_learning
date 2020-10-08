#include <condition_variable>
#include <mutex>
#include <queue>
#include <memory>

template<typename T>
class thread_safe_queue {
private:
    mutable std::mutex m;
    std::queue<T> q;
    std::condition_variable cv;

public:
    thread_safe_queue() {}
    thread_safe_queue(const thread_safe_queue& rhs)
    {
        std::scoped_lock l(rhs.m);
        m = rhs.m;
    }

    void push(const T& x)
    {
        std::scoped_lock l(m);
        q.push(std::move(x));
        cv.notify_one();
    }

    void wait_and_pop(T& x)
    {
        std::unique_lock l(m);
        cv.wait(l, [this]{ return !q.empty() });
        x = std::move(q.front());
        q.pop();
    }

    bool try_pop(T& x)
    {
        std::scoped_lock l(m);
        if (q.empty()) return false;
        x = std::move(q.front());
        q.pop();
        return true;
    }

    bool empty() const
    {
        std::scoped_lock l(m);
        return q.empty();
    }
};

template<typename T>
class A {
private:
    struct node {
        std::shared_ptr<T> val;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head;
    node* tail;
    std::mutex hm; // head mutex
    std::mutex tm; // tail mutex
    std::condition_variable cv;

    node* get_tail()
    {
        std::lock_guard<std::mutex> l(tm);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> oldHead = std::move(head);
        head = std::move(oldHead->next);
        return oldHead;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> l(hm);
        cv.wait(l, [&] { return head.get() != get_tail(); });
        return std::move(l);
    }

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> l(wait_for_data());
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& x)
    {
        std::unique_lock<std::mutex> l(wait_for_data());
        x = std::move(*head->val); // 即*(head->val)，->优先级高于*
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> l(hm);
        if (head.get() == get_tail()) return std::unique_ptr<node>();
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& x)
    {
        std::lock_guard<std::mutex> l(hm);
        if (head.get() == get_tail()) return std::unique_ptr<node>();
        x = std::move(*head->val);
        return pop_head();
    }
public:
    A() : head(new node), tail(head.get()) {}
    A(const A&) = delete;
    A& operator=(const A&) = delete;
    void push(T x)
    {
        std::shared_ptr<T> newVal(std::make_shared<T>(std::move(x)));
        std::unique_ptr<node> p(new node);
        {
            std::lock_guard<std::mutex> l(tm);
            tail->val = newVal;
            node* const newTail = p.get();
            tail->next = std::move(p);
            tail = newTail;
        }
        cv.notify_one();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node> oldHead = wait_pop_head();
        return oldHead->val;
    }

    void wait_and_pop(T& x)
    {
        std::unique_ptr<node> oldHead = wait_pop_head(x);
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> oldHead = try_pop_head();
        return oldHead ? oldHead->val : std::shared_ptr<T>();
    }

    bool try_pop(T& x)
    {
        std::unique_ptr<node> oldHead = try_pop_head(x);
        return oldHead;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> l(hm);
        return head.get() == get_tail();
    }
};