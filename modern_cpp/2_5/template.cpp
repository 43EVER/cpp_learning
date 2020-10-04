#include <iostream>
#include <vector>
#include <initializer_list>
#include <string>

template<bool T>
struct Node {
    int i = T;
};

using NewProcess = int(*)(void*);

int f(void*) {
    return 1;
}

template<typename T>
using MyVec = std::vector<T>;

template<typename T = int, typename U = int>
auto add(const T& x, const U& y) -> decltype(x + y) {
    return x + y;
}

int f2() {
    return 0;
}

template<typename T, typename... Args>
int f2(const T& value, Args... args) {
    return 1 + f2(args...);
}

template<typename T0, typename... T>
int f3(const T0& t0, T... t) {
    if constexpr (sizeof...(t) > 0) return 1 + f3(t...);;
    return 1;
}

template<typename T, typename... Ts>
auto f4(const T& value, Ts... args) {
    std::cout << value << std::endl;
    (void) std::initializer_list<T>{([&args] {
        std::cout << args << std::endl;
    }(), value)...};
}

template<typename ... T>
auto sum(T... t) {
    return (t + ...);
}

template<auto v>
class V {
public:
    using value = decltype(v);
};

int main() {
    NewProcess b = f;
    std::cout << b(nullptr) << std::endl;

    MyVec<int> a{1, 2, 3};

    std::cout << add(1, 2.4) << std::endl;

    std::cout << f2(1, "1", 3) << std::endl;

    std::cout << f3(1, 2, 3) << std::endl;

    f4(1, 1, 1);

    std::cout << sum(1, 2, 3.4, "123", 5, 6) << std::endl;

    V<123>::value;
}