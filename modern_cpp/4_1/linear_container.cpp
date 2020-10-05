#include <iostream>
#include <algorithm>
#include <array>
#include <forward_list>
using namespace std;

int main() {
    array<int, 6> a{1, 2, 3, 4, 5, 6};
    sort(a.begin(), a.end(), [](int a, int b) {
        return a > b;
    });
    for (auto&& i : a) {
        cout << i << " ";
    }
    cout << endl;

    
}