//
// Created by PNguyen on 17/04/2024.
//

#ifndef DSA_LIBRARIES_VECTOR_ITERATOR_HPP
#define DSA_LIBRARIES_VECTOR_ITERATOR_HPP
#include <iterator>

namespace DSALibraries::Containers
{
template <typename TIterator, typename TVector> class NormalVectorIter
{
  protected:
    TIterator _pElement;

  public:
    using ValueTypeAlias = std::iterator_traits<TIterator>::value_type;
    using ReferenceTypeAlias = std::iterator_traits<TIterator>::reference;
    using PointerTypeAlias = std::iterator_traits<TIterator>::pointer;
    using DifferenceTypeAlias = std::iterator_traits<TIterator>::difference_type;

    template <typename TIter>
    using is_convertible_from = std::enable_if_t<std::is_convertible<TIter, TIterator>::value>;

    // Adaptive alias
    using value_type [[maybe_unused]] = ValueTypeAlias;
    using reference [[maybe_unused]] = ReferenceTypeAlias;
    using pointer [[maybe_unused]] = PointerTypeAlias;
    using difference_type = DifferenceTypeAlias;
    using iterator_category [[maybe_unused]] = std::random_access_iterator_tag;

  public:
    constexpr NormalVectorIter() noexcept : _pElement(TIterator())
    {
    }

    constexpr explicit NormalVectorIter(const TIterator &element) noexcept : _pElement(element)
    {
    }

    template <typename TIter, typename = is_convertible_from<TIter>>
    constexpr NormalVectorIter(const NormalVectorIter<TIter, TVector> &other) noexcept : _pElement(other.Base())
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

    constexpr NormalVectorIter &operator++() noexcept
    {
        ++_pElement;
        return *this;
    }

    constexpr NormalVectorIter operator++(int) noexcept
    {
        return NormalVectorIter(_pElement++);
    }

    constexpr NormalVectorIter &operator--() noexcept
    {
        --_pElement;
        return *this;
    }

    constexpr NormalVectorIter operator--(int) noexcept
    {
        return NormalVectorIter(_pElement--);
    }

    constexpr NormalVectorIter &operator+=(difference_type n) noexcept
    {
        _pElement += n;
        return *this;
    }

    constexpr NormalVectorIter &operator-=(difference_type n) noexcept
    {
        _pElement -= n;
        return *this;
    }

    constexpr NormalVectorIter operator+(difference_type n) const noexcept
    {
        return NormalVectorIter(_pElement + n);
    }

    constexpr NormalVectorIter operator-(difference_type n) const noexcept
    {
        return NormalVectorIter(_pElement - n);
    }

    constexpr ReferenceTypeAlias operator[](difference_type n) const noexcept
    {
        return _pElement[n];
    }

    constexpr const TIterator &Base() const noexcept
    {
        return _pElement;
    }
};

template <typename TIterator, typename TVector>
inline bool operator==(const NormalVectorIter<TIterator, TVector> &lhs,
                       const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() == rhs.Base();
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator==(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                       const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() == rhs.Base();
}

template <typename TIterator, typename TVector>
inline bool operator!=(const NormalVectorIter<TIterator, TVector> &lhs,
                       const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return !(lhs == rhs);
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator!=(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                       const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return (lhs.Base() != rhs.Base());
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator<(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                      const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() < rhs.Base();
}

template <typename TIterator, typename TVector>
inline bool operator<(const NormalVectorIter<TIterator, TVector> &lhs,
                      const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() < rhs.Base();
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator>(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                      const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() > rhs.Base();
}

template <typename TIterator, typename TVector>
inline bool operator>(const NormalVectorIter<TIterator, TVector> &lhs,
                      const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() > rhs.Base();
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator<=(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                       const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() <= rhs.Base();
}

template <typename TIterator, typename TVector>
inline bool operator<=(const NormalVectorIter<TIterator, TVector> &lhs,
                       const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() <= rhs.Base();
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline bool operator>=(const NormalVectorIter<TIteratorLeft, TVector> &lhs,
                       const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() >= rhs.Base();
}

template <typename TIterator, typename TVector>
inline bool operator>=(const NormalVectorIter<TIterator, TVector> &lhs,
                       const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() >= rhs.Base();
}

template <typename TIteratorLeft, typename TIteratorRight, typename TVector>
inline typename NormalVectorIter<TIteratorLeft, TVector>::difference_type operator-(
    const NormalVectorIter<TIteratorLeft, TVector> &lhs, const NormalVectorIter<TIteratorRight, TVector> &rhs) noexcept
{
    return lhs.Base() - rhs.Base();
}

template <typename TIterator, typename TVector>
inline typename NormalVectorIter<TIterator, TVector>::difference_type operator-(
    const NormalVectorIter<TIterator, TVector> &lhs, const NormalVectorIter<TIterator, TVector> &rhs) noexcept
{
    return lhs.Base() - rhs.Base();
}

template <typename TIterator, typename TVector>
inline NormalVectorIter<TIterator, TVector> operator+(typename NormalVectorIter<TIterator, TVector>::difference_type n,
                                                      const NormalVectorIter<TIterator, TVector> &iter) noexcept
{
    return NormalVectorIter<TIterator, TVector>(iter.Base() + n);
}

} // namespace DSALibraries::Containers
#endif // DSALIBRARIES_VECTOR_ITERATOR_HPP
