#include <iostream>
#include <algorithm>
#include <functional>

namespace liuyang {

    template<typename T>
    class shared_ptr {
    private:
        int* counter = nullptr;
        T* ptr = nullptr;
        using Deleter_Typ = std::function<void(T*)>;

        Deleter_Typ deleter;

        void counter_add() {
            (*counter)++;
        }

        void counter_minus() {
            (*counter)--;
        }

        void _delete() {
            counter_minus();
            if (!(*counter)) {
                if (ptr) {
                    deleter(ptr);
                    delete counter;
                    ptr = nullptr;
                    counter = nullptr;
                }
            }
        }
    public:
        shared_ptr(T* ptr = nullptr, 
                Deleter_Typ deleter = 
                    [](T* a){
                        std::cout << "default delter" << std::endl; 
                        delete a;
                    }) 
                : counter(new int(1)), ptr(ptr), deleter(deleter) {
            std::cout << this << "(shared_ptr): 创建" << std::endl;
        }

        shared_ptr(const shared_ptr<T>& sptr) {
            std::cout << this << "(shared_ptr): 复制" << std::endl;
            this->deleter = sptr.deleter;
            counter = sptr.counter;
            ptr = sptr.ptr;
            counter_add();
        }

        T* operator->() {
            return ptr;
        }

        T& operator*() {
            return *ptr;
        }

        shared_ptr<T>& operator= (const shared_ptr<T>& sptr) {
            std::cout << this << "(shared_ptr): 赋值" << std::endl;

            if (this == &sptr) return *this;

            _delete();
            counter = sptr.counter;
            ptr = sptr.ptr;
            deleter = sptr->deleter;
            counter_add();

            return *this;
        }

        int use_count() {
            return *counter;
        }

        ~shared_ptr() {
            _delete();
        }
    };

    template<typename T, typename... Args>
    liuyang::shared_ptr<T> make_shared(Args&&... args) {
        return liuyang::shared_ptr<T>(new T(std::forward<Args>(args)...));
    }

}

struct Node {
    int a, b, c;
    Node(int a = 0, int b = 0, int c = 0) : a(a), b(b), c(c) { std::cout << this << "(Node): 创建" << std::endl; }
    ~Node() { std::cout << this << "(Node): 销毁" << std::endl; }
};

int main() {
    // auto a = liuyang::make_shared<Node>(1, 2, 3);
    // auto b = a;
    // std::cout << b.use_count() << std::endl;
    // {
    //     auto c = b;
    //     auto d = c;
    //     auto e = a;
    //     std::cout << a.use_count() << std::endl;
    // }
    // b.~shared_ptr();
    // std::cout << a.use_count() << std::endl;
    // std::cout << a->a << " " << a->b << " " << a->c << std::endl;
    // (*a).a = 100;
    // std::cout << a->a << " " << a->b << " " << a->c << std::endl;

    auto a = liuyang::shared_ptr<Node>(new Node[10], [](auto a) {
        std::cout << "custom deleter" << std::endl;
        delete[] a;
    });
}