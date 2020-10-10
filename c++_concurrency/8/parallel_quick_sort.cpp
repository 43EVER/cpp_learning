#include <iostream>
#include <list>
#include <algorithm>

template<typename T>
using list_typ = std::list<T>;

template<typename T>
void _sort(list_typ<T>& list)
{
    if (list.empty()) return;
    
    list_typ<T> res;
    res.splice(res.end(), list, list.begin());
    const T& firstVal = *res.begin();
    auto it = std::partition(list.begin(), list.end(), 
                                std::bind(std::less<T>(), 
                                            std::placeholders::_1, firstVal));
    
    list_typ<T> low;
    low.splice(low.end(), list, list.begin(), it);
    list_typ<T>& high = list;

    _sort(low);
    _sort(high);

    res.splice(res.end(), high);
    res.splice(res.begin(), low);
    list = std::move(res);
}

struct Node {
    static int copy_con_cnt;
    int x;
    Node(int x) : x(x) {}
    bool operator<(const Node& node) const
    {
        return x < node.x;
    }
    Node(const Node& node) {
        // std::cout << this << ": copy constructor" << std::endl;
        x = node.x;
        copy_con_cnt++;
    }
    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << node.x;
        return os;
    }
};

int Node::copy_con_cnt = 0;

int main()
{
    list_typ<Node> l{5, 1, 9, 2, 3, 0, 10};
    std::cout << Node::copy_con_cnt << std::endl;
    _sort(l);
    std::cout << Node::copy_con_cnt << std::endl;

    for (auto&& i : l) {
        std::cout << i << " ";
    }
}