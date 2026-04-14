//
// Created by MuXin on 2026/4/14.
//

export module f:string;

import std;
import :pack;

export namespace f {

template<typename... Args>
auto format_with_resource(std::format_string<Args...>&& fmt, std::pmr::memory_resource* resource, Args&& ...args) noexcept {
    auto buffer = std::pmr::string{resource};
    std::format_to(std::back_inserter(buffer), std::move(fmt), std::forward<Args>(args)...);
    return buffer;
}

template<typename... Args>
auto format(std::format_string<Args...>&& fmt, Args&& ...args) noexcept {
    using pak = pack<Args...>;
    if constexpr (std::same_as<typename pak::first, std::pmr::memory_resource*>)
        return format_with_resource(std::move(fmt), std::forward<Args>(args)...);
    else
        return format_with_resource(std::move(fmt), std::pmr::get_default_resource(), std::forward<Args>(args)...);
}

template<typename Alloc>
auto& cvt(
    const std::string_view mbs,
    std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc>& buf) {

    buf.resize(mbs.size());
    auto state = std::mbstate_t{};
    auto p = mbs.data();
    // ReSharper disable once CppDeprecatedEntity
    std::mbsrtowcs(buf.data(), &p, buf.length()+1, &state);
    return buf;
}

template<typename Alloc>
std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc> cvt(
    const std::basic_string<char, std::char_traits<char>, Alloc>& mbs) {

    auto p = mbs.data();
    auto state = std::mbstate_t{};
    auto buf = std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc>{
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
    std::wcsrtombs(buf.data(), &p, buf.length()+1, &state);
    return buf;
}

template<typename Alloc>
std::basic_string<char, std::char_traits<char>, Alloc> cvt(
    const std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc>& wcs) {
    auto p = wcs.data();
    auto state = std::mbstate_t{};
    auto buf = std::basic_string<char, std::char_traits<char>, Alloc>{
        wcs.size() * sizeof(wchar_t), '\x00', wcs.get_allocator()};
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::wcsrtombs(buf.data(), &p, buf.length()+1, &state));
    return std::move(buf);
}

}
