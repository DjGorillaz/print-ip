#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <list>
#include <tuple>

//String
template <typename T>
typename std::enable_if<std::is_same<std::string, T>::value>::type printIp(T&& v)
{
    std::cout << v << std::endl;
}

//Integral numbers
template <typename T>
typename std::enable_if<std::is_integral<T>::value>::type printIp(T&& v)
{
    unsigned char* begin = reinterpret_cast<unsigned char*>(&v);
    unsigned char* curr = begin + sizeof(T) - 1;
    std::string d = "";
    for(; curr >= begin; --curr)
    {
        std::cout << d << *curr - 0;
        d = '.';
    }
    std::cout << std::endl;
}

//Containers
template <typename T>
struct is_container: std::false_type { };

template <typename T>
struct is_container<std::list<T>>: std::true_type { };

template <typename T>
struct is_container<std::vector<T>>: std::true_type { };

template <typename T>
typename std::enable_if<is_container<T>::value>::type printIp(T&& t)
{
    std::string d = "";
    for(const auto& val: t)
    {
        std::cout << d << val;
        d = "..";
    }
    std::cout << std::endl;
}

//Print tuple
template <std::size_t idx, std::size_t fin, typename ... Args>
struct printTuple;

//Tuple with same types
template <typename T, typename ... Args>
struct is_same_tuple: std::false_type { };

template <typename T>
struct is_same_tuple<std::tuple<T>>: std::true_type { };

template <typename T, typename ... Args>
struct is_same_tuple<std::tuple<T, T, Args...>>: is_same_tuple<std::tuple<T, Args...>> { };

template <typename T>
typename std::enable_if<is_same_tuple<T>::value>::type printIp(T&& t)
{
    constexpr std::size_t size = std::tuple_size<T>::value;
    printTuple<0, size-1, T>{}(t);
}

//Print tuple
template <std::size_t idx, std::size_t fin, typename ... Args>
struct printTuple<idx, fin, std::tuple<Args...>>
{
    void operator()(const std::tuple<Args...>& t)
    {
       std::cout << std::get<idx>(t) << "..";
       printTuple<idx+1, fin, std::tuple<Args...>>{}(t);
    }
};

template <std::size_t fin, typename ... Args>
struct printTuple<fin, fin, std::tuple<Args...>>
{
    void operator()(const std::tuple<Args...>& t)
    {
        std::cout << std::get<fin>(t) << std::endl;
    }
};

int main()
{
    std::string str("1234.5678.0.9");
    std::vector<int> vec = {192, 168, 0, 1};
    std::list<long> list = {255, 255, 255, 0};
    auto tup = std::make_tuple(215, 121, 135, 120, 97);

    printIp(char(-1));
    printIp(short(0));
    printIp(int(2130706433));
    printIp(long long(8875824491850138409));
    printIp(std::move(str));
    printIp(std::move(vec));
    printIp(std::move(list));
    printIp(std::move(tup));

    return 0;
}
