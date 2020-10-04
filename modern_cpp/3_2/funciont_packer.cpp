#include <iostream>
#include <functional>

using foo = void(int);

void function(foo f = [](int value){std::cout << "value: " << value << std::endl;}) {
    f(1);
}

void fun(int value1, int value2) {
    std::cout << "value1: " << value1 << std::endl;
    std::cout << "value2: " << value2 << std::endl;
}

int main() {
    auto bindfoo = std::bind(fun, 10, std::placeholders::_1);
    bindfoo(1);
}