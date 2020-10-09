#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <utility>


template<typename T>
class A {
public:
    struct node {
        std::shared_ptr<T> val;
        node* next;
        node(const T& x) : val(std::make_shared<T>(x)), next(nullptr) {}
    };
    std::atomic<node*> head;
    std::atomic<unsigned> cnt;
    std::atomic<node*> toDel;

public:
    void push(const T& x)
    {
        const auto newNode = new node(x);
        newNode->next = head.load();
        while (!head.compare_exchange_weak(newNode->next, newNode));
    }

    std::shared_ptr<T> pop()
    {
        ++cnt;
        node* oldHead = head.load();
        while (!head.compare_exchange_weak(oldHead, oldHead->next));
        std::shared_ptr<T> res;
        if (oldHead) res.swap(oldHead->val);
        try_reclaim(oldHead);
        return res;
    }

private:
    static void deleteNodes(node* n)
    {
        while (n)
        {
            node* tmp = n->next;
            delete n;
            n = tmp;
        }
    }

    void try_reclaim(node* oldHead)
    {
        if (cnt == 1) {
            node* n = toDel.exchange(nullptr);
            if (--cnt == 0) {
                deleteNodes(n);
            } else if (n) {
                addToDel(n);
            }
        } else {
            addToDel(oldHead, oldHead);
            --cnt;
        }
    }

    void addToDel(node* n)
    {
        node* last = n;
        while (const auto tmp = last->next) last = tmp;
        addToDel(n, last);
    }

    void addToDel(node* first, node* last)
    {
        last->next = toDel;
        while (!toDel.compare_exchange_weak(last->next, first));
    }
};

int main()
{
    A<int> s;
    std::vector<std::thread> threads;
    for (int i = 0; i < 16; i++)
        threads.emplace_back([&]{
            if (i % 4 == 0) {
                s.push(i);
            } else {
                auto val = s.pop();
                if (val) std::cout << *val << std::endl;
            }
        });
    
}