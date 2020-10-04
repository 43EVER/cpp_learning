#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    Node() {
        cout << this << ": default constructor" << endl;
    }

    Node(const Node&) {
        cout << this << ": copy constructor" << endl;
    }

    Node(Node&&) {
        cout << this << ": move constructor" << endl;
    }

    Node& operator= (const Node&) {
        cout << this << ": assignment constructor" << endl;
        return *this;
    }

    Node& operator= (Node&&) {
        cout << this << ": assignment move construcotr" << endl;
        return *this;
    }
};

Node foo(bool ok) {
    Node a, b;
    if (ok) return a;
    else return b;
}

void reference(int& v) {
    std::cout << "左值" << std::endl;
}
void reference(int&& v) {
    std::cout << "右值" << std::endl;
}
template <typename T>
void pass(T&& v) {
    std::cout << "普通传参:";
    reference(std::forward<T>(v)); // 始终调用 reference(int&)
}



int main() {
    // perfect forwarding
    pass(*(new int));
    pass(1);
}