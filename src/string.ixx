//
// Created by MuXin on 2026/4/14.
//

export module f:string;

import std;
import :pack;

export namespace f {

template<typename Alloc>
auto cvt(
    const std::string_view mbs,
    std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc> buf) {

    buf.resize(mbs.size());
    auto state = std::mbstate_t{};
    auto p = mbs.data();
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::mbsrtowcs(buf.data(), &p, buf.length()+1, &state));
    return buf;
}

template<typename Alloc>
auto cvt(
    const std::wstring_view wcs,
    std::basic_string<char, std::char_traits<char>, Alloc> buf) {

    buf.resize(wcs.size() * sizeof(wchar_t));
    auto state = std::mbstate_t{};
    auto p = wcs.data();
    // ReSharper disable once CppDeprecatedEntity
    buf.resize(std::wcsrtombs(buf.data(), &p, buf.length()+1, &state));
    return buf;
}

}
