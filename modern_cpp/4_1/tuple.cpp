#include <tuple>
#include <iostream>
#include <variant>

using namespace std;

template<size_t n, typename... T>
constexpr variant<T...> _tuple_index(const tuple<T...>& tpl, size_t i) {
    if constexpr (n >= sizeof...(T))
        throw out_of_range("越界");
    if (i == n)
        return variant<T...>{ in_place_index<n>, std::get<n>(tpl) };
    return _tuple_index<(n < sizeof...(T) - 1 ? n + 1 : 0)>(tpl, i);
}

template<typename... T>
constexpr variant<T...> tuple_index(const tuple<T...>& tpl, size_t i) {
    return _tuple_index<0>(tpl, i);
}
template<typename T0, typename... Ts>
ostream& operator<< (ostream& s, variant<T0, Ts...> const & v) {
    visit([&](auto&& x){ s << x; }, v);
    return s;
}

int main() {
    auto p = make_tuple(3.8, 'A', "刘洋");
    auto [gpa, grade, name] = p;
    cout << gpa << " " << grade << " " << name << endl;
    tie(gpa, grade, name) = p;
    cout << gpa << " " << grade << " " << name << endl;
    cout << get<const char*>(p);

    int x;
    cin >> x;
    cout << tuple_index(p, x);
}