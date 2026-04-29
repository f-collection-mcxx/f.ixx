//
// Created by MuXin on 2026/4/14.
//

#pragma once
#include <f/exception.hpp>

namespace f {

constexpr void assert(const bool condition) {
    if (!condition)
        throw exception("bad assert");
}

}
