#ifndef DSA_LIBRARIES_CLIST_ITERATOR_HPP
#define DSA_LIBRARIES_CLIST_ITERATOR_HPP
#include "CListNode.hpp"

namespace DSALibraries::Containers
{
template <typename T> struct CListIterator
{
    CListNodeBase* NodeBase;

    using SelfAlias = CListIterator<T>;
    using NodeAlias = CListNode<T>;
    using ValueTypeAlias = T;
    using PointerAlias = T*;
    using ReferenceAlias = T&;

    CListIterator() noexcept : NodeBase()
    {
    }

    explicit CListIterator(CListNodeBase* nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<CListNode<T>*>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<CListNode<T>*>(this->NodeBase)->GetData();
    }

    SelfAlias& operator++() noexcept
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

    friend bool operator!=(const CListIterator<T>& lhs, const CListIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const CListIterator<T>& lhs, const CListIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return CListIterator(this->NodeBase->PointerNext);
        }
        else
        {
            return CListIterator(nullptr);
        }
    }
};

template <typename T> struct CListConstIterator
{

    using SelfAlias = CListConstIterator<T>;
    using NodeAlias = const CListNode<T>;
    using IteratorAlias = CListIterator<T>;

    using ValueTypeAlias = T;
    using PointerAlias = const T*;
    using ReferenceAlias = const T&;

    const CListNodeBase* NodeBase;

    CListConstIterator() noexcept : NodeBase()
    {
    }

    explicit CListConstIterator(const CListNodeBase* nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    CListConstIterator(const IteratorAlias& iter) noexcept : NodeBase(iter.NodeBase)
    {
    }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<NodeAlias*>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<NodeAlias*>(this->NodeBase)->GetData();
    }

    SelfAlias& operator++() noexcept
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

    friend bool operator!=(const SelfAlias& lhs, const SelfAlias& rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const SelfAlias& lhs, const SelfAlias& rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return CListConstIterator(this->NodeBase->PointerNext);
        }
        else
        {
            return CListConstIterator(nullptr);
        }
    }
};
} // namespace DSALibraries::Containers

#endif // !SINGLELINKEDLISTITERATOR_HPP