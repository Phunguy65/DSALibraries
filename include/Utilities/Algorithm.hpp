#ifndef DSA_UTILITIES_ALGORITHMS_HPP
#define DSA_UTILITIES_ALGORITHMS_HPP
#include <utility>
namespace DSALibraries::Utilities
{
template <typename TInputIterator, typename TOuputIterator>
TOuputIterator Copy(TInputIterator first, TInputIterator last, TOuputIterator result)
{
    while (first != last)
    {
        *result = *first;
        (void)++first;
        (void)++result;
    }
    return result;
}

template <typename TFirstInputIterator, typename TSecondInputIterator>
constexpr TSecondInputIterator Move(TFirstInputIterator first, TFirstInputIterator last, TSecondInputIterator result)
{
    while (first != last)
    {
        *result = std::move(*first);
        (void)++first;
        (void)++result;
    }
    return result;
}

template <typename TFirstInputIterator, typename TSecondInputIterator>
bool Equal(TFirstInputIterator first1, TFirstInputIterator last1, TSecondInputIterator first2)
{
    while (first1 != last1)
    {
        if (*first1 != *first2)
        {
            return false;
        }
        (void)++first1;
        (void)++first2;
    }
    return true;
}

template <typename TFirstInputIterator, typename TSecondInputIterator>
constexpr TSecondInputIterator MoveBackward(TFirstInputIterator first, TFirstInputIterator last,
                                            TSecondInputIterator result)
{
    while (first != last)
    {
        *(--result) = std::move(*(--last));
    }
    return result;
}

template <typename TOutputIterator, typename TSize, typename T>
TOutputIterator FillElements(TOutputIterator first, TSize count, const T &value)
{
    for (TSize i = 0; i < count; ++i)
    {
        *first = value;
        (void)++first;
    }
    return first;
}

template <typename TIterator, typename T> void Fill(TIterator first, TIterator last, const T &value)
{
    while (first != last)
    {
        *first = value;
        (void)++first;
    }
}
} // namespace DSALibraries::Utilities
#endif // DSA_UTILITIES_ALGORITHMS_HPP