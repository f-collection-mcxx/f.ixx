//
// Created by MuXin on 2026/4/14.
//

export module f:assert;
import :exception;

export namespace f {

void assert(const bool condition) {
    if (!condition)
        throw exception("bad assert");
}

}
