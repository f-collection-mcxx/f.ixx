//
// Created by MuXin on 2026/4/14.
//
#pragma once
#include <type_traits>
namespace f {

template<typename T, typename... Ts>
using first_t = T;

template<typename, typename... Ts>
struct last {
    using t = last<Ts...>::t;
};

template<typename T>
struct last<T> {
    using t = T;
};

template<typename... Ts>
using last_t = last<Ts...>::t;

template<typename... Ts>
using last_t = last<Ts...>::t;

template<typename...>
struct pack;

template<typename T, typename... Ts>
struct pack<T, Ts...> {
    constexpr static auto pack_size = sizeof...(Ts) + 1;

    using first = T;
    using last = std::conditional_t<(pack_size > 0), last_t<T, Ts...>, T>;

    template<size_t i>
    using at = std::conditional_t<i == 0, T, typename pack<Ts...>::template at<i-1>>;
};

template<>
struct pack<> {
    using first = void;
    using last = void;
    template<size_t>
    using at = void;
};

}
