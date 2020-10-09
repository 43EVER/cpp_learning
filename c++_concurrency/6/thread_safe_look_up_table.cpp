#include <iostream>
#include <algorithm>
#include <map>
#include <mutex>
#include <thread>
#include <memory>
#include <utility>
#include <shared_mutex>
#include <functional>
#include <list>
#include <vector>

template<typename K, typename V, typename Hash = std::hash<K>>
class A {
    class Bucket {
    public:
        std::list<std::pair<K, V>> data;
        mutable std::shared_mutex m;

        V value_for(const K& k, const V& v) const
        {
            std::shared_lock l(m);
            auto it = std::find_if(data.begin(), data.end(), 
                                    [&](auto&& x) { return x.first == k; });
            return it == data.end() ? v : it->second;
        }

        void add_or_update_mapping(const K& k, const V& v)
        {
            std::unique_lock l(m);
            auto it = std::find_if(data.begin(), data.end(), 
                                    [&](auto&& x) { return x.first == k; });
            if (it == data.end())
                data.emplace_back(k, v);
            else
                it->second = v;
        }

        void remove_mapping(const K& k)
        {
            std::unique_lock l(m);
            auto it = std::find_if(data.begin(), data.end(), 
                                    [&](auto&& x) { return x.first == k; });
            if (it != data.end())
                data.erase(it);
        }
    };

    std::vector<std::unique_ptr<Bucket>> buckets;
    Hash hasher;
    Bucket& get_bucket(const K& k) const
    {
        return *buckets[hasher(k) % buckets.size()];
    }
public:
    A(unsigned n = 19, const Hash& h = Hash{}) : buckets(n), hasher(h)
    {
        for (auto&& x : buckets)
            x.reset(new Bucket);
    }
    A(const A&) = delete;
    A& operator=(const A&) = delete;
    A(const A&&) = delete;
    A& operator=(const A&&) = delete;
    V value_for(const K& k, const V& v = V{}) const
    {
        return get_bucket(k).value_for(k, v);
    }

    void add_or_update_mapping(const K& k, const V& v)
    {
        get_bucket(k).add_or_update_mapping(k, v);
    }

    void remove_mapping(const K& k)
    {
        get_bucket(k).remove_mapping(k);
    }

    std::map<K, V> get_map() const
    {
        std::vector<std::unique_lock<std::shared_mutex>> l;
        for (auto&& x : buckets)
        {
            l.push_back(std::unique_lock<std::shared_mutex>(x->m));
        }
        std::map<K, V> res;
        for (auto&& x : buckets)
        {
            for (auto&& y : x->data) res.insert(y);
        }

        return res;
    }
};

int main()
{
    A<int, int> a;
    a.add_or_update_mapping(1, 1);
    a.add_or_update_mapping(2, 3);
    std::cout << a.value_for(2) << std::endl;
}