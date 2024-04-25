#ifndef DSA_LIBRARIES_SLIST_ITERATOR_HPP
#define DSA_LIBRARIES_SLIST_ITERATOR_HPP
#include "SListNode.hpp"
#include <iterator>

namespace DSALibraries::Containers
{
template <typename T> struct SListIterator
{
    SListNodeBase *NodeBase;

    using SelfAlias = SListIterator<T>;
    using NodeAlias = SListNode<T>;
    using ValueTypeAlias = T;
    using PointerAlias = T *;
    using ReferenceAlias = T &;

    // Adaptive alias
    using value_type = ValueTypeAlias;
    using reference = ReferenceAlias;
    using pointer = PointerAlias;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    SListIterator() noexcept : NodeBase()
    {
    }

    explicit SListIterator(SListNodeBase *nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    // SingleLinkedListIterator(const SingleLinkedListIterator& iter) noexcept : _nodeBase(iter._nodeBase)
    // {
    // }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<SListNode<T> *>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<SListNode<T> *>(this->NodeBase)->GetData();
    }

    SelfAlias &operator++() noexcept
    {
        this->NodeBase = this->NodeBase->PointerNext;
        return *this;
    }

    SelfAlias operator++(int) noexcept
    {
        SelfAlias temp(*this);
        this->NodeBase = this->NodeBase->PointerNext;
        return temp;
    }

    friend bool operator!=(const SListIterator<T> &lhs, const SListIterator<T> &rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const SListIterator<T> &lhs, const SListIterator<T> &rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return SListIterator(this->NodeBase->PointerNext);
        }
        else
        {
            return SListIterator(nullptr);
        }
    }
};

template <typename T> struct SListConstIterator
{

    using SelfAlias = SListConstIterator<T>;
    using NodeAlias = const SListNode<T>;
    using IteratorAlias = SListIterator<T>;

    using ValueTypeAlias = T;
    using PointerAlias = const T *;
    using ReferenceAlias = const T &;

    const SListNodeBase *NodeBase;

    SListConstIterator() noexcept : NodeBase()
    {
    }

    explicit SListConstIterator(const SListNodeBase *nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    SListConstIterator(const IteratorAlias &iter) noexcept : NodeBase(iter.NodeBase)
    {
    }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<NodeAlias *>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<NodeAlias *>(this->NodeBase)->GetData();
    }

    SelfAlias &operator++() noexcept
    {
        this->NodeBase = this->NodeBase->PointerNext;
        return *this;
    }

    SelfAlias operator++(int) noexcept
    {
        SelfAlias temp(*this);
        this->NodeBase = this->NodeBase->PointerNext;
        return temp;
    }

    friend bool operator!=(const SelfAlias &lhs, const SelfAlias &rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const SelfAlias &lhs, const SelfAlias &rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return SListConstIterator(this->NodeBase->PointerNext);
        }
        else
        {
            return SListConstIterator(nullptr);
        }
    }
};
} // namespace DSALibraries::Containers

#endif // !SINGLELINKEDLISTITERATOR_HPP