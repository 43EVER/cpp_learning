template<typename T>
struct iterator_traits {
    typedef typename T::value_type value_type;
};

template<typename T>
struct iterator_traits<T*> {
    typedef T value_type;
};

template<typename T>
struct iterator_traits<const T*> {
    typedef const T value_type;
};

int main()
{
    using value_type = iterator_traits<const int>::value_type;

    value_type a = 3;

}