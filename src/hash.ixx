//
// Created by MuXin on 2026/4/14.
//

export module f:hash;
import std;

export namespace f {

template<typename T>
struct hash;

template<typename S>
requires std::convertible_to<const S, std::string_view>
struct hash<S> {
    using is_transparent = void;
    size_t operator () (const auto& s) const noexcept {
        return std::hash<std::string_view>{}(std::string_view{s});
    }
};



}
