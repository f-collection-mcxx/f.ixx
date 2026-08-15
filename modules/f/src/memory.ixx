//
// Created by MuXin on 2025/11/23.
//

export module f:memory;
export import std;

export namespace f {

template<typename T>
struct dynamic_deleter {
    void operator () (T* ptr) const noexcept {
        ptr->~T();
        operator delete (ptr, size, align);
    }

    explicit
    dynamic_deleter(std::size_t size=sizeof(T), std::size_t align=alignof(T)):
        size{size},
        align{align}{}
    dynamic_deleter(dynamic_deleter&&) noexcept = default;
    dynamic_deleter& operator=(dynamic_deleter&&) noexcept = default;
    dynamic_deleter(const dynamic_deleter&) noexcept = default;
    dynamic_deleter& operator=(const dynamic_deleter&) noexcept = default;


    template<typename U>
    requires (std::derived_from<T, U> || std::derived_from<U, T>)
    // ReSharper disable once CppNonExplicitConvertingConstructor
    dynamic_deleter(const dynamic_deleter<U>& deleter):
        size{deleter.size},
        align{deleter.align}
    {}

    template<typename U>
    requires std::derived_from<U, T>
    // ReSharper disable once CppNonExplicitConvertingConstructor
    dynamic_deleter(const std::default_delete<U>&):
        size{sizeof(U)},
        align{alignof(U)}
    {}

    template<typename U>
    requires (std::derived_from<T, U> || std::derived_from<U, T>)
    dynamic_deleter& operator = (const dynamic_deleter<U>& deleter) {
        size = deleter.size;
        align = deleter.align;
        return *this;
    }

    std::size_t
        size;
    std::size_t
        align;
};

template<typename T>
using dynamic_unique_ptr = std::unique_ptr<T, dynamic_deleter<T>>;

template<typename B, typename D, typename Del>
requires std::derived_from<D, B> && (std::same_as<Del, std::default_delete<D>> || std::same_as<Del, dynamic_deleter<D>>)
auto unique_upcast(std::unique_ptr<D, Del>&& ptr) noexcept {
    return dynamic_unique_ptr<B>{
        static_cast<B*>(ptr.release()),
        dynamic_deleter<B>{ptr.get_deleter()}
    };
}

template<typename D, typename B>
requires std::derived_from<D, B>
auto unique_downcast(dynamic_unique_ptr<B>& ptr) noexcept -> std::optional<dynamic_unique_ptr<D>> {
    if (auto p = dynamic_cast<D*>(ptr.release()))
        return dynamic_unique_ptr<D>{p, dynamic_deleter<D>{ptr.get_deleter()}};
    else {
        ptr.reset(p);
        return std::nullopt;
    }
}

}