#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>

void foo(std::shared_ptr<int> i) {
    (*i)++;
}

void shared_ptr_test() {
    auto pointer = std::make_shared<int>(10);
    foo(pointer);
    std::cout << *pointer << std::endl;
    auto pointer1 = pointer;
    auto pointer2 = pointer1;
    std::cout << pointer.use_count() << std::endl;
    std::cout << pointer1.use_count() << std::endl;
    std::cout << pointer2.use_count() << std::endl;
    pointer2.reset();
    std::cout << pointer.use_count() << std::endl;
    std::cout << pointer1.use_count() << std::endl;
    std::cout << pointer2.use_count() << std::endl;
}

template<typename T, typename... Args>
std::unique_ptr<T> m_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Node {
    Node() { std::cout << "Node::Node" << std::endl; }
    ~Node() { std::cout << "Node::~Node" << std::endl; }
    void foo() { std::cout << "Node::foo" << std::endl; }
};

void f(const Node&) {
    std::cout << "f(const Node&)" << std::endl;
}

void unique_ptr_test() {
    auto pointer = m_make_unique<int>(1);
    // auto pointer2 = pointer;
    std::cout << *pointer << std::endl;
    (*pointer) = 10;
    std::cout << *pointer << std::endl;
    
    auto ptr = std::make_unique<Node>();
    auto p1 = std::move(ptr);
    if (p1) p1->foo();
    {
        std::unique_ptr<Node> p2(std::move(p1));
        f(*p2);
        if (p2) {
            std::cout << "p2" << std::endl;
            p2->foo();
        }
        if (p1) {
            p1->foo();
            p1->foo();
        }
        p1 = std::move(p2);
        std::cout << "p2 被销毁" << std::endl;
    }
    if (p1) {
        std::cout << "p1" << std::endl;
        p1->foo();
    }
}

struct A;
struct B;

struct B {
    std::weak_ptr<A> ptr;
    B() { std::cout << this << "(B): 被创建" << std::endl; }
    ~B() { std::cout << this << "(B): 被销毁" << std::endl; }
};

struct A {
    std::weak_ptr<B> ptr;
    A() { std::cout << this << "(A): 被创建" << std::endl; }
    ~A() { std::cout << this << "(A): 被销毁" << std::endl; }
};

void weak_ptr_test() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    {
        a->ptr = b;
        b->ptr = a;
        std::cout << a.use_count() << std::endl;
        auto c = a->ptr.lock();
        std::cout << a.use_count() << std::endl;
    }
}

void custom_deleter() {
    auto a = std::shared_ptr<A>(new A[10], [](auto a) {
        std::cout << "custom deleter" << std::endl;
        delete[] a;
    });
    auto b = std::shared_ptr<A>(new A[10]);
}

int main() {
    // shared_ptr_test();
    // unique_ptr_test();
    // weak_ptr_test();
    custom_deleter();
}