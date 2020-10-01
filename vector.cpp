#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template<typename T>
using F = vector<T>;

int main()
{
    stack<int, F<int>> a;
    a.push(1);
    cout << a.top() << endl;
    a.pop();

}