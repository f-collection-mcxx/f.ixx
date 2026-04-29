//
// Created by MuXin on 2025/11/23.
//
#pragma once
#include <memory_resource>
#include <ranges>
#include <optional>

namespace f {

template<typename SmallPool, typename LargePool, std::size_t Threshold>
class size_based_pool final: public std::pmr::memory_resource {
public:
    explicit
    size_based_pool(memory_resource* upstream=std::pmr::get_default_resource()):
        _small{upstream},
        _large{upstream}
    {}

private:
    void* do_allocate(
        const size_t _Bytes,
        const size_t _Align) override {
        if (_Bytes > Threshold)
            return _large.allocate(_Bytes, _Align);
        return _small.allocate(_Bytes, _Align);
    }
    void do_deallocate(
        void* _Ptr,
        const size_t _Bytes,
        const size_t _Align) override {
        if (_Bytes > Threshold)
            _large.deallocate(_Ptr, _Bytes, _Align);
        _small.deallocate(_Ptr, _Bytes, _Align);
    }
    bool do_is_equal(
        const std::pmr::memory_resource& _That) const noexcept override {
        return this == &_That;
    }

    SmallPool
        _small;
    LargePool
        _large;
};

template<std::size_t Threshold=3 * 1024 * 1024>
using sync_sized_pool = size_based_pool<std::pmr::synchronized_pool_resource, std::pmr::synchronized_pool_resource, Threshold>;

template<std::size_t Threshold=3 * 1024 * 1024>
using sized_pool = size_based_pool<std::pmr::unsynchronized_pool_resource, std::pmr::unsynchronized_pool_resource, Threshold>;


template<typename T>
struct deleter {
    void operator () (T* ptr) const noexcept {
        ptr->~T();
        resource->deallocate(ptr, sizeof(T), alignof(T));
    }

    explicit
    deleter(std::pmr::memory_resource* resource=std::pmr::get_default_resource()):
        resource(resource){}

    std::pmr::memory_resource*
        resource;
};

template<typename T>
struct deleter<T[]> {
    void operator () (T* ptr) const noexcept {
        for (auto i: std::views::iota(0ull, count))
            (ptr + i) -> ~T();
        resource->deallocate(ptr, sizeof(T) * count, alignof(T));
    }

    explicit
    deleter(const std::size_t count, std::pmr::memory_resource* resource=std::pmr::get_default_resource()):
        count{count}, resource(resource){}

    std::size_t
        count;
    std::pmr::memory_resource*
        resource;
};

template<typename T>
struct dynamic_deleter {
    void operator () (T* ptr) const noexcept {
        ptr->~T();
        resource->deallocate(ptr, size, align);
    }

    explicit
    dynamic_deleter(std::pmr::memory_resource* resource=std::pmr::get_default_resource(), std::size_t size=sizeof(T), std::size_t align=alignof(T)):
        resource(resource),
        size{size},
        align{align}{}


    template<typename U>
    requires (std::derived_from<T, U> || std::derived_from<U, T>)
    // ReSharper disable once CppNonExplicitConvertingConstructor
    dynamic_deleter(const dynamic_deleter<U>& deleter):
        resource(deleter.resource),
        size{deleter.size},
        align{deleter.align}
    {}

    template<std::derived_from<T> U>
    explicit
    dynamic_deleter(const deleter<U>& deleter):
        resource(deleter.resource),
        size{sizeof(U)},
        align{alignof(U)}
    {}

    template<std::derived_from<T> U>
    dynamic_deleter& operator = (const dynamic_deleter<U>& deleter) {
        resource = deleter.resource;
        size = deleter.size;
        align = deleter.align;
        return *this;
    }

    std::pmr::memory_resource*
        resource;
    std::size_t
        size;
    std::size_t
        align;
};

template<typename T>
using unique_ptr = std::unique_ptr<T, deleter<T>>;

template<typename T>
using dynamic_unique_ptr = std::unique_ptr<T, dynamic_deleter<T>>;

template<typename T, typename... Args>
auto make_unique(std::pmr::memory_resource* mem_res, Args&&... args) {
    const auto ptr = mem_res->allocate(sizeof(T), alignof(T));
    if constexpr (std::constructible_from<T, Args..., std::pmr::memory_resource*>)
        return unique_ptr<T>{
            new (ptr) T {std::forward<Args>(args)..., mem_res},
            deleter<T>{mem_res}
        };
    else if constexpr (std::constructible_from<T, std::pmr::memory_resource*, Args...>)
        return unique_ptr<T>{
            new (ptr) T {mem_res, std::forward<Args>(args)...},
            deleter<T>{mem_res}
        };
    else
        return unique_ptr<T>{
            new (ptr) T {std::forward<Args>(args)...},
            deleter<T>{mem_res}
        };

}

template<typename T, typename ...Args>
auto make_unique_array(std::size_t count, std::pmr::memory_resource* mem_res, Args&&... args) {
    const auto ptr = static_cast<T*>(mem_res->allocate(sizeof(T) * count, alignof(T)));
    for (auto i: std::views::iota(0ull, count)) {
        if constexpr (std::constructible_from<T, Args...>)
            new (ptr + i) T {std::forward<Args>(args)...};
        else if constexpr (std::constructible_from<T, std::pmr::memory_resource*, Args...>)
            new (ptr + i) T {mem_res, std::forward<Args>(args)...};
        else
            new (ptr + i) T {std::forward<Args>(args)..., mem_res};
    }
    return unique_ptr<T[]>{
        ptr,
        deleter<T[]>{count, mem_res}
    };
}

template<typename B, typename D, typename Del>
requires std::derived_from<D, B> && (std::same_as<Del, deleter<D>> || std::same_as<Del, dynamic_deleter<D>>)
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