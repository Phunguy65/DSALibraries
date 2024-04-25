#ifndef DSA_UTILITIES_HELPERS_HPP
#define DSA_UTILITIES_HELPERS_HPP
#include "Algorithm.hpp"
#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>
namespace DSALibraries::Utilities
{

template <typename TForwardIterator, typename T>
TForwardIterator UninitializedFill(TForwardIterator first, TForwardIterator last, const T &value)
{
    using ValueType = typename std::iterator_traits<TForwardIterator>::value_type;

    TForwardIterator current = first;

    try
    {
        while (current != last)
        {
            ::new (static_cast<void *>(std::addressof(*current))) ValueType(value);
            ++current;
        }
        return current;
    }
    catch (...)
    {
        while (first != current)
        {
            first->~ValueType();
            ++first;
        }
        throw;
    }
}

/*
@description: UninitializedFillElements is a helper function that fills a range of elements with a given value.
@param {TForwardIterator} first: The beginning of the range.
@param {TSize} count: The number of elements to fill.
@param {TIterator} value: The value to fill the elements with.
@return {TForwardIterator}: The iterator pointing to the end of the range.
*/
template <typename TForwardIterator, typename TSize, typename T>
TForwardIterator UninitializedFillElements(TForwardIterator first, TSize count, const T &value)
{
    using ValueType = typename std::iterator_traits<TForwardIterator>::value_type;

    TForwardIterator current = first;

    try
    {
        while (count > 0)
        {
            ::new (static_cast<void *>(std::addressof(*current))) ValueType(value);
            ++current;
            --count;
        }
        return current;
    }
    catch (...)
    {
        while (first != current)
        {
            first->~ValueType();
            ++first;
        }
        throw;
    }
}

/*
@description: DestroyAt is a helper function that destroys an element.
@param {TIterator} element: The element to destroy.
@return {void}
@note: This function is a template function that uses SFINAE to determine if the element is an array or not.
*/
template <typename T> constexpr void DestroyAt(T *element)
{
    if constexpr (std::is_array_v<T>)
    {
        for (auto &item : *element)
        {
            (DestroyAt)(std::addressof(item));
        }
    }
    else
    {
        element->~T();
    }
}

/*
@description: DestroyElements is a helper function that destroys a range of elements.
@param {TIterator} begin: The beginning of the range.
@param {TSize} count: The number of elements to destroy.
@return {TIterator}: The iterator pointing to the end of the range.
*/
template <typename TIterator, typename TSize> constexpr TIterator DestroyElements(TIterator begin, TSize count)
{
    for (; count > 0; (void)begin, --count)
    {
        DestroyAt(std::addressof(*begin));
        ++begin;
    }
    return begin;
}

/*
@description: DestroyRange is a helper function that destroys a range of elements.
@param {TIterator} begin: The beginning of the range.
@param {TIterator} end: The end of the range.
@return {void}
*/
template <typename TIterator> constexpr void Destroy(TIterator begin, TIterator end)
{
    for (; begin != end; ++begin)
    {
        DestroyAt(std::addressof(*begin));
    }
}

/*
@description: UninitializedCopy is a helper function that copies a range of elements to a new range.
@param {TInputIterator} first: The beginning of the range to copy.
@param {TInputIterator} last: The end of the range to copy.
@param {TOutputIterator} result: The beginning of the range to copy to.
@return {TOutputIterator}: The iterator pointing to the end of the range.
@note: This function is a template function that uses SFINAE to determine if the value type of the output iterator is
trivially constructible.
*/
template <typename TInputIterator, typename TOutputIterator>
typename std::enable_if<
    !std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
    TOutputIterator>::type
UninitializedCopy(TInputIterator first, TInputIterator last, TOutputIterator result)
{
    using ValueType = typename std::iterator_traits<TOutputIterator>::value_type;

    TOutputIterator current = result;

    try
    {
        while (first != last)
        {
            ::new (static_cast<void *>(std::addressof(*current))) ValueType(*first);
            ++first;
            ++current;
        }
        return current;
    }
    catch (...)
    {
        while (result != current)
        {
            result->~ValueType();
            ++result;
        }
        throw;
    }
}

/**
 * @brief This function is a helper function that copies a range of elements to a new range.
 *
 * @tparam TInputIterator The type of the input iterator.
 * @tparam TOutputIterator The type of the output iterator.
 * @param first The beginning of the range to copy.
 * @param last The end of the range to copy.
 * @param result The beginning of the range to copy to.
 * @return std::enable_if<
 * std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
 * TOutputIterator>::type
 */
template <typename TInputIterator, typename TOutputIterator>
typename std::enable_if<
    std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
    TOutputIterator>::type
UninitializedCopy(TInputIterator first, TInputIterator last, TOutputIterator result)
{
    return DSALibraries::Utilities::Copy(first, last, result);
}

/**
 * @brief
 *
 * @tparam TInputIterator
 * @tparam TSize
 * @tparam TOutputIterator
 * @param first
 * @param count
 * @param result
 * @return TOutputIterator
 */
template <typename TInputIterator, typename TSize, typename TOutputIterator>
TOutputIterator UninitializedCopyElements(TInputIterator first, TSize count, TOutputIterator result)
{
    return Utilities::UninitializedCopy(first, first + count, result);
}

/**
 * @brief Create a Default At object
 *
 * @tparam T
 * @return std::enable_if<std::is_trivially_constructible<TIterator>::value, void>::type
 */
template <typename T>
typename std::enable_if<std::is_trivially_constructible<T>::value, void>::type CreateDefaultAt(T *);

/**
 * @brief Create a Default At object
 *
 * @tparam T
 * @param pValue
 * @return std::enable_if<!std::is_trivially_constructible<TIterator>::value, void>::type
 */
template <typename T>
typename std::enable_if<!std::is_trivially_constructible<T>::value, void>::type CreateDefaultAt(T *pValue)
{
    ::new (static_cast<void *>(pValue)) T();
}

/**
 * @brief Create a Value At object
 *
 * @tparam T
 * @param pValue
 */
template <typename T> void CreateValueAt(T *pValue)
{
    ::new (static_cast<void *>(pValue)) T();
}

/**
 * @brief Create a Value At object
 *
 * @tparam T
 * @param pValue
 * @param value
 */
template <typename T> void CreateValueAt(T *pValue, const T &value)
{
    ::new (static_cast<void *>(pValue)) T(value);
}

template <typename T> void CreateCopyAt(T *pValue, T &&value)
{
    ::new (static_cast<void *>(pValue)) T(std::move(value));
}

/**
 * @brief
 *
 * @tparam TInputIterator
 * @tparam TOutputIterator
 * @param first
 * @param last
 * @param result
 * @return std::enable_if<
 * !std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
 * TOutputIterator>::type
 */
template <typename TInputIterator, typename TOutputIterator>
typename std::enable_if<
    !std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
    TOutputIterator>::type
UninitializedMove(TInputIterator first, TInputIterator last, TOutputIterator result)
{
    using ValueType = typename std::iterator_traits<TOutputIterator>::value_type;

    TOutputIterator current = result;

    while (first != last)
    {
        ::new (static_cast<void *>(std::addressof(*current))) ValueType(std::move(*first));
        ++first;
        ++current;
    }

    return current;
}

template <typename TInputIterator, typename TOutputIterator>
typename std::enable_if<
    std::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value,
    TOutputIterator>::type
UninitializedMove(TInputIterator first, TInputIterator last, TOutputIterator result)
{
    return DSALibraries::Utilities::Move(first, last, result);
}

template <typename TForwardIterator> void UnInitializedDefaultConstruct(TForwardIterator first, TForwardIterator last)
{
    using ValueType = typename std::iterator_traits<TForwardIterator>::value_type;

    TForwardIterator current = first;

    try
    {
        while (current != last)
        {
            ::new (const_cast<void *>(static_cast<const volatile void *>(std::addressof(*current)))) ValueType();
            ++current;
        }
    }
    catch (...)
    {
        Destroy(first, current);
        throw;
    }
}

template <typename TForwardIterator, typename TSize>
TForwardIterator UnInitializedDefaultConstructElements(TForwardIterator first, TSize count)
{
    using ValueType = typename std::iterator_traits<TForwardIterator>::value_type;

    TForwardIterator current = first;

    try
    {
        while (count > 0)
        {
            ::new (const_cast<void *>(static_cast<const volatile void *>(std::addressof(*current)))) ValueType();
            ++current;
            --count;
        }
        return current;
    }
    catch (...)
    {
        Destroy(first, current);
        throw;
    }
}

template <typename TVector> struct ShrinkToFitHelper
{
    constexpr static bool DoIt(TVector &vector) noexcept
    {
        try
        {
            TVector(std::move(vector.GetBegin()), std::move(vector.GetEnd()), vector.GetAllocator()).Swap(vector);
            return true;
        }
        catch (...)
        {
            return false;
        }
        return false;
    }
};
} // namespace DSALibraries::Utilities
#endif // DSA_UTILITIES_HELPERS_HPP