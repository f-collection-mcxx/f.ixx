//
// Created by MuXin on 2026/4/14.
//
#pragma once
#include <string>

namespace f {

struct string_hasher {
    using is_transparent = void;
    template<typename Str>
    requires std::convertible_to<const Str, std::string_view>
    size_t operator () (const Str& s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};



}
