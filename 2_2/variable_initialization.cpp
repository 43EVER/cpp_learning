#include <iostream>
#include <vector>
#include <tuple>
#include <string>

auto f() {
    return std::make_tuple(1, 2.3, "456");
}

int main()
{
    int a = 2;
    if (int i = 0; i != a) {
        std::cout << "fuck" << std::endl;
    }

    std::vector<int> vec{1, 2, 3, 4};

    auto [x, y, z] = f();
    std::cout << x << " " << y << " " << z << std::endl;
}