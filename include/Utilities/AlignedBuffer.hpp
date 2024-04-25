//
// Created by PNguyen on 23/04/2024.
//

#ifndef DSALIBRARIES_ALIGNEDBUFFER_HPP
#define DSALIBRARIES_ALIGNEDBUFFER_HPP
#include <type_traits>
namespace DSALibraries::Utilities
{
template <typename T> struct AlignedBuffer : std::aligned_storage<sizeof(T), alignof(T)>
{
    struct Tp
    {
        T Value;
    };

    alignas(alignof(Tp::Value)) unsigned char Storage[sizeof(T)];

    AlignedBuffer() = default;

    AlignedBuffer(std::nullptr_t) noexcept
    {
    }

    void *Address() noexcept
    {
        return static_cast<void *>(&Storage);
    }

    [[nodiscard]] const void *Address() const noexcept
    {
        return static_cast<const void *>(&Storage);
    }

    T *Pointer() noexcept
    {
        return static_cast<T *>(this->Address());
    }

    const T *Pointer() const noexcept
    {
        return static_cast<const T *>(this->Address());
    }
};
} // namespace DSALibraries::Utilities
#endif // DSALIBRARIES_ALIGNEDBUFFER_HPP
