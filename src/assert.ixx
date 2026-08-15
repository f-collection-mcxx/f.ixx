//
// Created by MuXin on 2026/4/14.
//

export module f:assert;
import :exception;

export namespace f {
constexpr struct assert_t {
    void operator >> (const bool condition) const {
        if (!condition)
            throw exception("bad assert");
    }
} assert;
}
