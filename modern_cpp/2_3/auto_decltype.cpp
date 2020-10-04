#include <iostream>
#include <vector>
#include <string>

// void f(auto x, auto y) 不能用于函数传参

template<typename T, typename U>
auto add2(T x, U y) -> decltype(x + y) {
    return x + y;
}

template<typename T, typename U>
auto add(T x, U y) {
    return x + y;
}

std::string lookup1() {
    return std::string();
}

std::string& lookup2() {
    auto l = *(new std::string());
    return l;
}

decltype(auto) look_up_a_string_1() {
    return lookup1();
}

decltype(auto) look_up_a_string_2() {
    return lookup2();
}

int main() {
    std::vector<int> vec{1};
    if (auto it = vec.begin(); it != vec.end()) {
        std::cout << *it << std::endl;
    }

    using TYPE = decltype(1);
    TYPE a = 1;

    auto x = 1;
    auto y = 2;
    decltype(x+y) z;
    if (std::is_same<decltype(x), int>::value)
        std::cout << "type x == int" << std::endl;
    if (std::is_same<decltype(x), float>::value)
        std::cout << "type x == float" << std::endl;
    if (std::is_same<decltype(x), decltype(z)>::value)
        std::cout << "type z == type x" << std::endl;

    std::cout << add2(1, 2) << std::endl;
}