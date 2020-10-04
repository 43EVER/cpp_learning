#include <iostream>
#include <vector>

template<typename T>
auto print_type_info(const T& t) {
    if constexpr (std::is_integral<T>::value) {
        return t + 1;
    } else {
        return t + 0.001;
    }
}

int main() {
    std::vector<int> vec{1, 2, 3, 4};
    if (auto itr = vec.begin(); itr != vec.end()) {
        *itr = 999;
    }

    for (auto it : vec) {
        std::cout << it << " ";
    }
}