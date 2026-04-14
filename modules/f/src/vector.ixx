//
// Created by MuXin on 2026/4/14.
//

export module f:vector;
import std;
import :memory;

export namespace f {

template<typename T>
requires std::is_polymorphic_v<T>
class dynamic_vector: std::pmr::vector<T*> {
    using super = std::pmr::vector<T*>;
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
        super::emplace_back(p);
        _size_n_align.emplace_back(sizeof(T), alignof(T));
        return *p;
    }
    auto release_back() {
        auto b = super::back();
        auto [sz, alg] = _size_n_align.back();
        super::pop_back();
        _size_n_align.pop_back();

        return dynamic_unique_ptr<T>{b, dynamic_deleter<T>{super::get_allocator().resource(), sz, alg}};
    }

    void clear() {
        for (auto&& [ptr, pak]: std::views::zip(*this, _size_n_align)) {
            ptr->~T();
            auto& [sz, alg] = pak;
            super::get_allocator().deallocate_bytes(ptr, sz, alg);
        }
        super::clear();
        _size_n_align.clear();
    }
    void swap(dynamic_vector& other) noexcept {
        super::swap(other);
        _size_n_align.swap(other._size_n_align);
    }

    explicit dynamic_vector(const std::pmr::polymorphic_allocator<>& alloc=std::pmr::get_default_resource()):
        super{alloc}, _size_n_align{alloc}{}
    dynamic_vector(dynamic_vector&& expired) noexcept {
        swap(expired);
    }
    dynamic_vector& operator = (dynamic_vector&& expired) noexcept {
        swap(expired);
        return *this;
    }

    dynamic_vector(const dynamic_vector&)=delete;
    void operator = (const dynamic_vector&)=delete;

private:
    std::pmr::vector<std::pair<std::size_t, std::size_t>>
        _size_n_align{super::get_allocator()};
};

}
