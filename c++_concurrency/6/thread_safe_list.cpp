#include <memory>
#include <mutex>
#include <algorithm>

template<typename T>
class A {
private:
    struct node {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node() : next() {}
        node(const T& x) : data(std::make_shared(x)) {}
    };
    node head;

public:
    A() {}
    ~A() { }
};