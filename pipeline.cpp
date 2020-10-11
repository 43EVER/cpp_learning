#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <condition_variable>
#include <mutex>
#include <type_traits>
#include <functional>

struct Node {
    int x, y;
    int fuck(int a) const
    {
        std::cout << x << " " << y << " " << a << std::endl;
        return 999;
    }

    void test(int& a) const
    {
        a = 999;
    }
};

template<typename _Callable, typename Object, typename... Args, 
typename = typename std::enable_if<std::is_member_function_pointer<_Callable>::value, _Callable>::type>
auto proxy_call(_Callable&& fun, Object&& obj, Args&&... args)
{
    return (obj->*fun)(std::forward<Args>(args)...);
}

template<typename _Callable, typename... Args>
auto proxy_call(_Callable&& fun, Args&&... args)
{
    return fun(std::forward<Args>(args)...);
}

template<typename _Callable, typename... Args>
auto call(_Callable&& fun, Args&&... args)
{
    return std::bind(std::forward<_Callable>(fun), std::forward<Args>(args)...)();
}


int fuck(int* ptr, int value)
{
    std::cout << "void fuck(int*, int)" << std::endl;
    return 1;
}

int main()
{
    const Node node{1, 2};
    std::cout << proxy_call(&Node::fuck, &node, 1) << std::endl;
    int a = 0;
    call(&Node::test, &node, a);

    int b = proxy_call(&fuck, &a, a);
}