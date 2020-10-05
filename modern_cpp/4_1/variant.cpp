#include <iostream>
#include <variant>
#include <string>
#include <cmath>
using namespace std;

using IntFloatString = variant<int, float, string>;

void ff(IntFloatString& s) {
    visit([&](auto&& x) {
        cout << x << endl;
    }, s);
}

int main() {
    IntFloatString i, f, s;
    i = 1;
    f = 3.1415f;
    s = "刘洋";

    auto x = get<float>(f);
    cout << x << endl;

    ff(i);
}
