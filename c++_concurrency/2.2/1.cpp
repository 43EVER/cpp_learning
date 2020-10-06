#include <iostream>
#include <thread>
#include <string>
#include <cstdio>
#include <memory>

void f(int i, std::string const& s)
{
    std::printf("void f(%d, %s)", i, s.c_str());  
}

struct Node {
    Node() { std::cout << this << "(Node): default constructor" << std::endl; }
    Node(Node const&) { std::cout << this << "(Node): copy constructor" << std::endl; }
    Node(Node const&&) { std::cout << this << "(Node): move constructor" << std::endl; }
    Node& operator=(Node const&) 
    { 
        std::cout << this << "(Node): assignment operator" << std::endl; 
        return *this; 
    }
    Node& operator=(Node const&&)
    {
        std::cout << this << "(Node): move assignment operator" << std::endl; 
        return *this;
    }
    void test(int p1, int p2)
    {
        std::printf("%ul(Node): void test(%d(int), %d(int))\n", this, p1, p2);
    }
};

void update(Node* add, Node& data)
{
    std::cout << "add: " << add << std::endl;
}

int main()
{
    // std::thread t(f, 1, std::string("123"));
    // t.join();
    Node a;
    /*
     * 会粗暴的复制一遍，然后以右值传递
    */
    // std::thread t(update, &a, a);
    std::thread t(update, &a, std::ref(a));
    t.join();

    std::thread t2(&Node::test, &a, 1, 100);
    t2.join();

    
}