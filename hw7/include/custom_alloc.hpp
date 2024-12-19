#ifndef CUSTOM_ALLOCATOR_HPP
#define CUSTOM_ALLOCATOR_HPP

#include <cstddef>
#include <memory>
#include <iostream>
#include <limits>
// обычный аллокатор
template <typename T>
class CustomAllocator {
public:
    using value_type = T;

    CustomAllocator() noexcept = default;

    template <typename U>
    CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();
        T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));
        return ptr;
    }

    void deallocate(T* ptr, std::size_t n) noexcept {
        ::operator delete(ptr);
    }

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        ::new (static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();
    }

    template <typename U>
    struct rebind {
        using other = CustomAllocator<U>;
    };

    bool operator==(const CustomAllocator&) const noexcept { return true; }
    bool operator!=(const CustomAllocator&) const noexcept { return false; }
};

#endif // CUSTOM_ALLOCATOR_HPP
