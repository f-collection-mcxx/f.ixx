//
// Created by MuXin on 2026/4/14.
//

export module f:deque;
import std;
import :memory;

export namespace f {


template<typename T>
requires std::is_polymorphic_v<T>
class dynamic_deque: public std::pmr::deque<T*> {
    using super = std::pmr::deque<T*>;
public:
    using super::at;
    using super::operator[];
    using super::size;
    using super::empty;
    using super::get_allocator;
    using super::front;
    using super::back;
    using super::begin;
    using super::end;


    template<typename U, typename ...Args>
    requires std::derived_from<U, T> && (
        std::constructible_from<U, Args...>
        || std::constructible_from<U, std::pmr::polymorphic_allocator<>, Args...>
        || std::constructible_from<U, Args..., std::pmr::polymorphic_allocator<>>)
    U& emplace_back(Args&&... args) {
        U* p = nullptr;
        if constexpr (std::constructible_from<U, Args...>)
            p = get_allocator().template new_object<U>(std::forward<Args>(args)...);
        else if constexpr (std::constructible_from<U, std::pmr::polymorphic_allocator<>, Args...>)
            p = get_allocator().template new_object<U>(get_allocator(), std::forward<Args>(args)...);
        else
            p = get_allocator().template new_object<U>(std::forward<Args>(args)..., get_allocator());
        super::emplace_back(f::dynamic_unique_ptr<T>{p, f::dynamic_deleter<U>{}});
        _size.emplace_back(sizeof(T));
        _align.emplace_back(alignof(T));
        return *p;
    }

    template<typename U, typename ...Args>
    requires std::derived_from<U, T> && (
        std::constructible_from<U, Args...>
        || std::constructible_from<U, std::pmr::polymorphic_allocator<>, Args...>
        || std::constructible_from<U, Args..., std::pmr::polymorphic_allocator<>>)
    U& emplace_front(Args&&... args) {
        U* p = nullptr;
        if constexpr (std::constructible_from<U, Args...>)
            p = get_allocator().template new_object<U>(std::forward<Args>(args)...);
        else if constexpr (std::constructible_from<U, std::pmr::polymorphic_allocator<>, Args...>)
            p = get_allocator().template new_object<U>(get_allocator(), std::forward<Args>(args)...);
        else
            p = get_allocator().template new_object<U>(std::forward<Args>(args)..., get_allocator());
        super::emplace_front(f::dynamic_unique_ptr<T>{p, f::dynamic_deleter<U>{}});
        _size.emplace_front(sizeof(T));
        _align.emplace_front(alignof(T));
        return *p;
    }

    auto release_back() {
        auto b = super::back();
        auto sz = _size.back();
        auto alg = _align.back();
        super::pop_back();
        _size.pop_back();
        _align.pop_back();

        return dynamic_unique_ptr<T>{b, dynamic_deleter<T>{super::get_allocator().resource(), sz, alg}};
    }

    auto release_front() {
        auto f = super::front();
        auto sz = _size.front();
        auto alg = _align.front();
        super::pop_front();
        _size.pop_front();
        _align.pop_front();

        return dynamic_unique_ptr<T>{f, dynamic_deleter<T>{super::get_allocator().resource(), sz, alg}};
    }

    void clear() {
        for (auto&& [ptr, sz, alg]: std::views::zip(*this, _size, _align)) {
            ptr->~T();
            super::get_allocator().deallocate_bytes(ptr, sz, alg);
        }
        super::clear();
        _size.clear();
        _align.clear();
    }
    void swap(dynamic_deque& other) noexcept {
        super::swap(other);
        _size.swap(other._size);
        _align.swap(other._align);
    }

    explicit dynamic_deque(const std::pmr::polymorphic_allocator<>& alloc=std::pmr::get_default_resource()):
        super{alloc}, _size{alloc}, _align{alloc}{}
    dynamic_deque(dynamic_deque&& expired) noexcept {
        swap(expired);
    }
    dynamic_deque& operator = (dynamic_deque&& expired) noexcept {
        swap(expired);
        return *this;
    }

    dynamic_deque(const dynamic_deque&)=delete;
    void operator = (const dynamic_deque&)=delete;

private:
    std::pmr::deque<std::size_t>
        _size,
        _align;
};

}
