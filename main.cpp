#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>
#include <utility>
#include <functional>

namespace ipaddr
{
static std::stringstream tuplerez;
template<typename U>
void print_element(const U& value) 
{
    tuplerez << value << ".";
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type
print_ip(const T& value)
{
    std::stringstream ipS;
    for(short byte = (sizeof(value)*8 - 8); byte >= 0; byte -= 8)
    {
        ipS << ((value >> byte) & 0xff) << ((byte > 0) ? "." : ""); 
    }
    std::cout << ipS.str() << std::endl;
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type 
print_ip(const T& value) 
{
    std::cout << value << "\n";
}

template<typename T>
struct container : std::false_type {};

template<typename T, typename Alloc>
struct container<std::vector<T, Alloc>> : std::true_type {};

template<typename T, typename Alloc>
struct container<std::list<T, Alloc>> : std::true_type {};

template<typename T>
typename std::enable_if<container<T>::value>::type
print_ip(const T& value)
{
    auto l_value = value.begin();
    while (l_value != value.end())
    {
        std::cout << *l_value;
        l_value++;
        if (l_value != value.end()) std::cout << ".";
    }
    std::cout << std::endl;
}


template<typename... Ts>
struct all_same;

template<>
struct all_same<> : std::true_type {};

template<typename T>
struct all_same<T> : std::true_type {};

template<typename T, typename U, typename... Rest>
struct all_same<T, U, Rest...> 
    : std::conditional<std::is_same<T,U>::value && all_same<U, Rest...>::value,
                       std::true_type,
                       std::false_type>::type {};

template<typename... Ts>
typename std::enable_if<all_same<Ts...>::value>::type
print_ip(const std::tuple<Ts...> tpl) {
    std::apply([](const auto&... args) 
    {
        ((print_element(args)), ...);
    }
    , tpl);
    std::string res = tuplerez.str();
    res.pop_back();
    tuplerez.clear();
    std::cout << res <<std::endl;
}


}//endofnamespace



int main()
{
    ipaddr::print_ip(int8_t{-1});
    ipaddr::print_ip(int16_t{0});
    ipaddr::print_ip(int32_t{2130706433});
    ipaddr::print_ip(int64_t{8875824491850138409});
    ipaddr::print_ip(std::string{"Hello, World!"});
    ipaddr::print_ip(std::vector<int>{100, 200, 300, 400});
    ipaddr::print_ip(std::list<short>{400, 300, 200, 100});
    ipaddr::print_ip(std::make_tuple(123, 456, 789, 0));
}