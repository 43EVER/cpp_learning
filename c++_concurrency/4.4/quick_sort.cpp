#include <iostream>
#include <algorithm>
#include <list>
#include <future>
#include <thread>
#include <cstdlib>
#include <vector>
#include <shared_mutex>
#include <set>

template<typename T>
std::list<T> f(std::list<T> v)
{
    if (v.empty()) return v;
    std::list<T> res;
    res.splice(res.begin(), v, v.begin());
    const T& firstVal = *res.begin();

    auto it = std::partition(v.begin(), v.end(), [&](const T& x) { return x < firstVal; });
    std::list<T> low;
    low.splice(low.end(), v, v.begin(), it);
    auto l(f(std::move(low)));
    auto r(f(std::move(v)));


    res.splice(res.end(), r);
    res.splice(res.begin(), l);
    return res;
}


std::set<std::thread::id> s;
std::shared_mutex m;

template<typename T>
std::list<T> pf(std::list<T> v)
{
    std::unique_lock lk(m);
    s.insert(std::this_thread::get_id());
    lk.unlock();

    if (v.empty()) {
        lk.lock();
        s.erase(std::this_thread::get_id());
        return v;
    }
    
    std::list<T> res;
    res.splice(res.begin(), v, v.begin());
    const T& firstVal = *res.begin();
    auto it = std::partition(v.begin(), v.end(), [&](const T& x) { return x < firstVal; });

    std::list<T> low;
    low.splice(low.end(), v, v.begin(), it);
    auto r(pf(std::move(v)));
    res.splice(res.end(), r);

    if (low.size() > 1e5 && s.size() < 16) {
        std::future<std::list<T>> l(std::async(&pf<T>, std::move(low)));
        res.splice(res.begin(), l.get());
    } else {
        auto l(pf(std::move(low)));
        res.splice(res.begin(), l);
    }
    lk.lock();
    s.erase(std::this_thread::get_id());
    return res;
}

#define PARALLEL

int main()
{
    std::list<int> l;
    for (int i = 0; i < 1e6; i++) {
        l.push_back(rand() % int(1e7));
    }

#ifndef PARALLEL
    auto start = clock();
    {
        auto res1 = f(std::move(l));
    }
    std::cout << (clock() - start) * 1000.0 / CLOCKS_PER_SEC << std::endl;
#endif

#ifdef PARALLEL
    auto start = clock();
    // std::sort(vec.begin(), vec.end());
    {
        auto res1 = pf(std::move(l));
    }
    std::cout << (clock() - start) * 1000.0 / CLOCKS_PER_SEC << std::endl;
    std::cout << s.size() << std::endl;
#endif
}