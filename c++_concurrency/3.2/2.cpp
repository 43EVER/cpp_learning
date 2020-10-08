#include <iostream>
#include <exception>
#include <algorithm>
#include <mutex>
#include <memory>

struct empty_stack : std::exception {
    const char* what() const throw();
};

template<typename T>
class threadsafe_stack {
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack&);
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value);
    std::shared_ptr<T> pop();
    void pop(T& value);
    bool empty() const;
};

struct Node {
    std::mutex m;

    friend void swap(Node& lhs, Node& rhs)
    {
        if (&lhs == &rhs)
            return;
        std::lock(lhs.m, rhs.m);
    }
};

void _swap(Node& lhs, Node& rhs)
{
    std::lock(  )
}

int main()
{

}