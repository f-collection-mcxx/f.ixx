//
// Created by MuXin on 2025/11/23.
//
#pragma once
#include <stdexcept>
#include <typeindex>
#include <any>

namespace f {

struct method_not_implemented: std::runtime_error {
    explicit
    method_not_implemented(const std::string_view name):
        runtime_error{std::format("method not implemented: {}", name)},
        method_name{name}
    {}

    std::string
        method_name;
};

/// 存储一个类型在运行时所有*常见*的行为（如果有）
/// 包括：
/// - 默认构造
/// - 复制构造
/// - 移动构造
/// - 析构
/// - 复制赋值
/// - 移动赋值
class type_t {
    using default_constructor_t = auto (std::byte*) -> void;
    using destructor_t          = auto (std::byte*) -> void;
    using copier_t              = auto (std::byte*, const std::byte*) -> void;
    using mover_t               = auto (std::byte*, std::byte*) -> void;

public:
    auto size() const noexcept -> std::size_t {
        return _size;
    }
    auto align() const noexcept -> std::size_t {
        return _align;
    }

    void make(std::byte* obj) const {
        if (!_default_constructor)
            throw method_not_implemented{"Default Constructor"};
        _default_constructor(obj);
    }
    auto try_make(std::byte* obj) const -> bool {
        if (!_default_constructor)
            return false;
        _default_constructor(obj);
        return true;
    }
    void make(std::byte* obj, const std::byte* prototype) const {
        if (!_copy_constructor)
            throw method_not_implemented{"Copy Constructor"};
        _copy_constructor(obj, prototype);
    }
    auto try_make(std::byte* obj, const std::byte* prototype) const -> bool {
        if (!_copy_constructor)
            return false;
        _copy_constructor(obj, prototype);
        return true;
    }
    void make(std::byte* obj, std::byte* expired) const {
        if (!_move_constructor)
            throw method_not_implemented{"Move Constructor"};
        _move_constructor(obj, expired);
    }
    auto try_make(std::byte* obj, std::byte* expired) const -> bool {
        if (!_move_constructor)
            return false;
        _move_constructor(obj, expired);
        return true;
    }

    void destroy(std::byte* obj) const {
        _destructor(obj);
    }

    void copy(std::byte* obj, const std::byte* prototype) const {
        if (!_copier)
            throw method_not_implemented{"Copy Assignment"};
        _copier(obj, prototype);
    }
    auto try_copy(std::byte* obj, const std::byte* prototype) const -> bool {
        if (!_copier)
            return false;
        _copier(obj, prototype);
        return true;
    }
    void move(std::byte* obj, std::byte* expired) const {
        if (!_mover)
            throw method_not_implemented{"Move Assignment"};
        _mover(obj, expired);
    }
    auto try_move(std::byte* obj, std::byte* expired) const -> bool {
        if (!_mover)
            return false;
        _mover(obj, expired);
        return true;
    }

    auto operator == (const type_t& other) const noexcept {
        return _info == other._info;
    }
    auto operator == (const std::type_index& other) const noexcept {
        return _info == other;
    }
    auto operator == (const std::type_info& other) const noexcept {
        return _info == other;
    }


    template<typename T>
    static
    auto of() noexcept {
        if constexpr (std::same_as<T, void>) {
            return nullptr;
        }
        else {
            static type_t unit{std::in_place_type<T>};
            return &unit;
        }
    }

    type_t() = default;
    type_t(type_t&&) noexcept = default;
    type_t(const type_t&) noexcept = default;
    template<typename T>
    explicit
    type_t(std::in_place_type_t<T>):
            _info{typeid(T)},
            _size{sizeof(T)},
            _align{alignof(T)},
            _destructor{[](std::byte* ptr) {
                if constexpr (requires{ptr -> ~T();}) {
                    ptr -> ~T();
                }
            }}{
        if constexpr (std::is_default_constructible_v<T>)
            _default_constructor = [](std::byte* ptr) {
                new (ptr) T {};
            };
        if constexpr (std::is_copy_constructible_v<T>)
            _copy_constructor = [](std::byte* dst, const std::byte* src) {
                new (dst) T {reinterpret_cast<const T&>(*src)};
            };
        if constexpr (std::is_move_constructible_v<T>)
            _move_constructor = [](std::byte* dst, std::byte* src) {
                new (dst) T {std::move(reinterpret_cast<T&>(*src))};
            };
        if constexpr (std::is_copy_assignable_v<T>)
            _copier = [](std::byte* dst, const std::byte* src) {
                reinterpret_cast<T&>(*dst) =
                    reinterpret_cast<const T&>(*src);
            };
        if constexpr (std::is_move_assignable_v<T>)
            _mover = [](std::byte* dst, std::byte* src) {
                reinterpret_cast<T&>(*dst) =
                    std::move(reinterpret_cast<T&>(*src));
            };
    }

    type_t& operator = (const type_t&) =default;
    type_t& operator = (type_t&&) noexcept =default;


    explicit
    operator std::type_index () const noexcept {
        return _info;
    }

private:
    std::type_index
        _info{typeid(void)};
    std::size_t
        _size{};
    std::size_t
        _align{};
    destructor_t*
        _destructor{};

    default_constructor_t*
        _default_constructor{};
    copier_t*
        _copy_constructor{};
    mover_t*
        _move_constructor{};
    copier_t*
        _copier{};
    mover_t*
        _mover{};
};


template<typename T, typename U>
requires std::is_class_v<U>
bool is_instance([[maybe_unused]] U& obj) noexcept {
    return typeid(obj) == typeid(T);
}

template<typename D, typename T>
requires std::is_class_v<T>
auto& as(T& obj) noexcept {
    if constexpr (std::is_const_v<T>) {
        using B = std::remove_const_t<T>;
        static_assert(std::derived_from<D, B>);
        return dynamic_cast<const D&>(obj);
    }
    else {
        using B = T;
        static_assert(std::derived_from<D, B>);
        return dynamic_cast<D&>(obj);
    }
}

template<typename D, typename T>
requires std::is_class_v<T>
auto as(T* obj) noexcept {
    if constexpr (std::is_const_v<std::remove_pointer_t<T>>) {
        using B = std::remove_const_t<std::remove_pointer_t<T>>;
        static_assert(std::derived_from<D, B>);
        return dynamic_cast<const D*>(obj);
    }
    else {
        using B = std::remove_pointer_t<T>;
        static_assert(std::derived_from<D, B>);
        return dynamic_cast<D*>(obj);
    }
}

class any {
public:
    template<typename T, typename U>
    auto&& as(this U&& self)  {
        if (self._type == type_t::of<void>())
            throw std::bad_any_cast{};
        if (self._type != type_t::of<T>())
            throw std::bad_any_cast{};

        if constexpr (std::is_const_v<std::remove_reference_t<U>>)
            return *std::launder(reinterpret_cast<const T*>(self._buffer.data()));
        else
            return *std::launder(reinterpret_cast<T*>(self._buffer.data()));
    }

    template<typename T, typename ...Args>
    T& emplace(Args&& ... args) {
        clear();
        _type = type_t::of<T>();
        _buffer.resize(_type->size());
        const auto p = new (_buffer.data()) T {std::forward<Args>(args)...};
        return *p;
    }

    void clear() {
        if (_type) {
            _type->destroy(_buffer.data());
            _buffer.clear();
        }
        _type = nullptr;
    }


    any& operator = (const any& other) {
        if (this == &other)
            return *this;
        if (_type == other._type)
            _type->copy(_buffer.data(), other._buffer.data());
        else {
            clear();
            _type = other._type;
            _buffer.resize(_type->size());
            _type->make(_buffer.data(), other._buffer.data());
        }
        return *this;
    }

    any& operator = (any&& other) noexcept {
        _type->destroy(_buffer.data());
        std::swap(_type, other._type);
        std::swap(_buffer, other._buffer);
        return *this;
    }


    template<typename T, typename ...Args>
    explicit
    any(std::in_place_type_t<T>, Args&& ... args):
        _type{type_t::of<T>()}{
        emplace<T>(std::forward<Args>(args)...);
    }
    any() = default;
    any(any&& other) noexcept:
        _buffer{std::move(other._buffer)},
        _type{other._type}{}
    any(const any& other) :
        _buffer{other._buffer.size()},
        _type{other._type}{
        _type->make(_buffer.data(), other._buffer.data());
    }
    ~any() {
        clear();
    }



private:
    std::pmr::vector<std::byte>
        _buffer{};
    type_t*
        _type{};
};



}

template<>
struct std::hash<f::type_t> {
    using is_transparent = void;
    auto operator () (const auto& obj) noexcept {
        return std::hash<std::type_index>{}(
            static_cast<std::type_index>(obj));
    }
};