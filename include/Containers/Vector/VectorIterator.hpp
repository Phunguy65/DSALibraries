//
// Created by PNguyen on 17/04/2024.
//

#ifndef DSALIBRARIES_VECTOR_ITERATOR_HPP
#define DSALIBRARIES_VECTOR_ITERATOR_HPP
#include <iterator>

namespace DSALibraries::Containers
{
template <typename T, typename TVector> class NormalVectorIter
{
    T _pElement;
    using SelfAlias = NormalVectorIter<T, TVector>;
    using ValueTypeAlias = std::iterator_traits<T>::value_type;
    using ReferenceTypeAlias = std::iterator_traits<T>::reference;
    using PointerTypeAlias = std::iterator_traits<T>::pointer;
    using DifferenceTypeAlias = std::iterator_traits<T>::difference_type;

    // Adaptive alias
    using value_type [[maybe_unused]] = ValueTypeAlias;
    using reference [[maybe_unused]] = ReferenceTypeAlias;
    using pointer [[maybe_unused]] = PointerTypeAlias;
    using difference_type = DifferenceTypeAlias;
    using iterator_category [[maybe_unused]] = std::random_access_iterator_tag;

  public:
    constexpr NormalVectorIter() noexcept : _pElement(T())
    {
    }

    constexpr explicit NormalVectorIter(const T &element) noexcept : _pElement(element)
    {
    }

    template <typename TIterator, typename = std::enable_if_t<std::is_convertible<TIterator, T>::value>>
    constexpr NormalVectorIter(const NormalVectorIter<TIterator, TVector> &iter) noexcept : _pElement(iter.Base())
    {
    }

    constexpr ReferenceTypeAlias operator*() const noexcept
    {
        return *_pElement;
    }

    constexpr PointerTypeAlias operator->() const noexcept
    {
        return _pElement;
    }

    constexpr SelfAlias &operator++() const noexcept
    {
        ++_pElement;
        return *this;
    }

    constexpr SelfAlias operator++(int) noexcept
    {
        SelfAlias temp(*this);
        ++_pElement;
        return temp;
    }

    constexpr SelfAlias &operator--() const noexcept
    {
        --_pElement;
        return *this;
    }

    constexpr SelfAlias operator--(int) noexcept
    {
        SelfAlias temp(*this);
        --_pElement;
        return temp;
    }

    constexpr SelfAlias &operator+=(difference_type n) const noexcept
    {
        _pElement += n;
        return *this;
    }

    constexpr SelfAlias &operator-=(difference_type n) const noexcept
    {
        _pElement -= n;
        return *this;
    }

    constexpr SelfAlias operator+(difference_type n) const noexcept
    {
        return SelfAlias(_pElement + n);
    }

    constexpr SelfAlias operator-(difference_type n) const noexcept
    {
        return SelfAlias(_pElement - n);
    }

    constexpr ReferenceTypeAlias operator[](difference_type n) const noexcept
    {
        return _pElement[n];
    }

    constexpr const T &Base() const noexcept
    {
        return _pElement;
    }
};
} // namespace DSALibraries::Containers
#endif // DSALIBRARIES_VECTOR_ITERATOR_HPP
