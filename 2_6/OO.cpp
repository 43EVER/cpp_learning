#include <iostream>
#include <algorithm>
#include <map>

class Base {
public:
    int value1;
    int value2;
    Base() {
        value1 = 1;
    }
    Base(int value) : Base() {
        value2 = value;
    }

    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }

    virtual void foo2() final {
        std::cout << "Base::foo2" << std::endl;
    }
};

class Subclass : public Base {
public:
    using Base::Base;    // 继承构造
    virtual void foo() override {
        std::cout << "Subclass::Base" << std::endl;
    }

    // virtual void foo(int) override; 

    // virtual void foo2() override {}
};

class Magic {
public:
    Magic() = default;
    Magic& operator=(const Magic&) = delete;
};

enum class new_enum : unsigned int {
    value1,
    value2 
};

// 习题
template <typename Key, typename Value, typename F>
void update(std::map<Key, Value>& m, F foo) {
    for (auto&& [key, value] : m) value = foo(key);
}

void test() {
    std::map<std::string, long long int> m {
        {"a", 1},
        {"b", 2},
        {"c", 3}
    };
    update(m, [](std::string key) {
        return std::hash<std::string>{}(key);
    });
    for (auto&& [key, value] : m)
        std::cout << key << ":" << value << std::endl;
}

template<typename... Args>
auto average(Args ... args) {
    return (... - args);
}

int main() {
    test();

    std::cout << average(1, 2, 3, 4) << std::endl;

}

