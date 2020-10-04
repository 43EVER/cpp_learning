#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>

using namespace std;

int main() {
    auto important = make_unique<int>(1);
    auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
        return x + y + v1 + (*v2);
    };
    cout << add(3, 4) << endl;

    auto add2 = [](const auto& x, const auto& y) {
        return x + y;
    };

    cout << add2(1, 2) << endl;
}