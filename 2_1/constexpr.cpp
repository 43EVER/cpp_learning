#include <iostream>
#include <ctime>
#include <cstdlib>

constexpr int len_foo_constexpr() {
    return 5;
}

constexpr int fib(const int n) {
    return n == 1 || n == 2 ? 1 : fib(n - 1) + fib(n - 2);
}

int _fib(int n) {
    return n == 1 || n == 2 ? 1 : _fib(n - 1) + _fib(n - 2);
}

int main() {
    constexpr int a = 1;
    int arr[a];

    auto start = clock();
    int res = fib(43);
    std::cout << res << std::endl;
    std::cout << 1.0 * (clock() - start) / CLOCKS_PER_SEC << std::endl;

    start = clock();
    std::cout << _fib(43) << std::endl;
    std::cout << 1.0 * (clock() - start) / CLOCKS_PER_SEC << std::endl;
}