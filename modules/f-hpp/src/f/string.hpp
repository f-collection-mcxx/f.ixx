//
// Created by MuXin on 2026/4/14.
//

#pragma once
#include <format>

namespace f {

template<typename... Args>
auto format(std::format_string<Args...>&& fmt, std::pmr::memory_resource* resource, Args&& ...args) noexcept {
    auto buffer = std::pmr::string{resource};
    std::format_to(std::back_inserter(buffer), std::move(fmt), std::forward<Args>(args)...);
    return buffer;
}

template<typename T, typename... Args>
requires (!std::convertible_to<T, std::pmr::memory_resource*>)
auto format(std::format_string<Args...>&& fmt, T&& t, Args&& ...args) noexcept {
    return format(std::move(fmt), std::pmr::get_default_resource(), std::forward<T>(t), std::forward<Args>(args)...);
}

template<typename Alloc>
auto& cvt(
    const std::string_view mbs,
    std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc>& buf) {

    buf.resize(mbs.size());
    auto state = std::mbstate_t{};
    auto p = mbs.data();
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::mbsrtowcs(buf.data(), &p, buf.length()+1, &state));
    return buf;
}

inline std::pmr::wstring cvt(
    const std::pmr::string& mbs) {

    auto p = mbs.data();
    auto state = std::mbstate_t{};
    auto buf = std::pmr::wstring{
        mbs.size(), L'\x00', mbs.get_allocator()};
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::mbsrtowcs(buf.data(), &p, buf.length()+1, &state));
    return std::move(buf);
}

template<typename Alloc>
auto& cvt(
    const std::wstring_view wcs,
    std::basic_string<char, std::char_traits<char>, Alloc>& buf) {

    buf.resize(wcs.size() * sizeof(wchar_t));
    auto state = std::mbstate_t{};
    auto p = wcs.data();
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::wcsrtombs(buf.data(), &p, buf.length()+1, &state));
    return buf;
}


inline std::pmr::string cvt(
    const std::pmr::wstring& wcs) {
    auto p = wcs.data();
    auto state = std::mbstate_t{};
    auto buf = std::pmr::string{
        wcs.size() * sizeof(wchar_t), '\x00', wcs.get_allocator()};
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::wcsrtombs(buf.data(), &p, buf.length()+1, &state));
    return std::move(buf);
}

}
