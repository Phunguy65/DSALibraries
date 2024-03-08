#ifndef DSA_LIBRARIES_UTILITIES_ALLOCATOR_HPP
#define DSA_LIBRARIES_UTILITIES_ALLOCATOR_HPP
#include <limits>
#include <memory>
#include <new>
#include <utility>
namespace DSALibraries::Utilities
{
template <typename T> class Allocator;

template <> class Allocator<void>
{
  public:
    using value_type = void;
    using pointer = void*;
    using const_pointer = void const*;
    using void_pointer = void*;
    using const_void_pointer = void const*;

    template <typename U> struct Rebind
    {
        typedef Allocator<U> other;
    };
};

template <typename T> class Allocator
{
  public:
    using value_type = T;

    using pointer = value_type*;
    using const_pointer = typename std::pointer_traits<pointer>::template rebind<value_type const>;
    using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;

    using difference_type = typename std::pointer_traits<pointer>::difference_type;
    using size_type = std::make_unsigned_t<difference_type>;

    template <typename U> struct rebind
    {
        typedef Allocator<U> other;
    };

    Allocator() noexcept = default; // not required, unless used

    template <typename U> Allocator(Allocator<U> const&) noexcept
    {
    }

    constexpr value_type* // Use pointer if pointer is not a value_type*
    allocate(std::size_t n)
    {
        return static_cast<value_type*>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(value_type* pValueType, std::size_t) noexcept // Use pointer if pointer is not a value_type*
    {
        ::operator delete(pValueType);
    }

    template <typename U, typename... Args> void construct(U* p, Args&&... args)
    {
        ::new ((void*)p) U(std::forward<Args>(args)...);
    }

    template <typename U> void destroy(U* p)
    {
        p->~U();
    }

    [[nodiscard]] std::size_t max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    Allocator select_on_container_copy_construction() const noexcept
    {
        return *this;
    }

    using is_always_equal = std::true_type;
};

template <typename T, typename U> bool operator==(Allocator<T> const&, Allocator<U> const&) noexcept
{
    return true;
}

template <typename T, typename U> bool operator!=(Allocator<T> const& x, Allocator<U> const& y) noexcept
{
    return !(x == y);
}
} // namespace DSALibraries::Utilities
#endif