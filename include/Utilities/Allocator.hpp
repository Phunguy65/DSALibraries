#ifndef DSA_LIBRARIES_UTILITIES_ALLOCATOR_HPP
#define DSA_LIBRARIES_UTILITIES_ALLOCATOR_HPP
#include <limits>
#include <memory>
#include <new>
#include <utility>
namespace DSALibraries::Utilities
{
template <typename T> class Allocator;

template <typename T> class Allocator
{
  public:
    using value_type = T;
    using pointer = value_type *;
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

    template <typename U> constexpr Allocator(Allocator<U> const &) noexcept
    {
    }

    [[nodiscard]] value_type *allocate(std::size_t n,
                                       [[maybe_unused]] const void * = static_cast<const void *>(nullptr))
    {
        return static_cast<value_type *>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(value_type *pValueType,
                    [[maybe_unused]] std::size_t) noexcept // Use pointer if pointer is not a value_type*
    {
        ::operator delete(pValueType);
    }

    template <typename U, typename... Args> void construct(U *p, Args &&...args)
    {
        ::new ((void *)p) U(std::forward<Args>(args)...);
    }

    template <typename U> void destroy(U *p)
    {
        p->~U();
    }

    [[nodiscard]] constexpr std::size_t max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    Allocator select_on_container_copy_construction() const noexcept
    {
        return *this;
    }

    using is_always_equal [[maybe_unused]] = std::true_type;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
};

template <typename T, typename U> bool operator==(Allocator<T> const &, Allocator<U> const &) noexcept
{
    return true;
}

template <typename T, typename U> bool operator!=(Allocator<T> const &x, Allocator<U> const &y) noexcept
{
    return !(x == y);
}
} // namespace DSALibraries::Utilities
#endif