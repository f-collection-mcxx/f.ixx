//
// Created by MuXin on 2026/4/14.
//

export module f:pack;
import std;

export namespace f {

template<typename T, typename... Ts>
using first_t = T;

template<typename, typename... Ts>
struct last {
    using t = last<Ts...>;
};

template<typename T>
struct last<T> {
    using t = T;
};

template<typename... Ts>
using last_t = last<Ts...>::t;

template<typename... Ts>
using last_t = last<Ts...>::t;

template<typename T, typename... Ts>
struct pack {
    constexpr static auto pack_size = sizeof...(Ts) + 1;

    using first = T;
    using last = std::conditional_t<(pack_size > 1), typename pack<Ts...>::last, T>;

    template<size_t i>
    requires (i < pack_size)
    using at = std::conditional_t<i == 0, T, typename pack<Ts...>::template at<i-1>>;

};

}
