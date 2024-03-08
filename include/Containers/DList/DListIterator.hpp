#ifndef DSA_LIBRARIES_DLIST_ITERATOR_HPP
#define DSA_LIBRARIES_DLIST_ITERATOR_HPP
#include "DListNode.hpp"
namespace DSALibraries::Containers
{
template <typename T> struct DListIterator
{
    DListNodeBase* NodeBase;

    using SelfAlias = DListIterator<T>;
    using NodeAlias = DListNode<T>;
    using ValueTypeAlias = T;
    using PointerAlias = T*;
    using ReferenceAlias = T&;

    DListIterator() noexcept : NodeBase()
    {
    }

    explicit DListIterator(DListNodeBase* nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<DListNode<T>*>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<DListNode<T>*>(this->NodeBase)->GetData();
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

    SelfAlias& operator--() noexcept
    {
        this->NodeBase = this->NodeBase->PointerPrev;
        return *this;
    }

    SelfAlias operator--(int) noexcept
    {
        SelfAlias temp(*this);
        this->NodeBase = this->NodeBase->PointerPrev;
        return temp;
    }

    friend bool operator!=(const DListIterator<T>& lhs, const DListIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const DListIterator<T>& lhs, const DListIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return SelfAlias(this->NodeBase->PointerNext);
        }
        else
        {
            return SelfAlias();
        }
    }

    SelfAlias PointerPrev() const noexcept
    {
        if (this->NodeBase)
        {
            return SelfAlias(this->NodeBase->PointerPrev);
        }
        else
        {
            return SelfAlias();
        }
    }
};

template <typename T> struct DListConstIterator
{
    const DListNodeBase* NodeBase;

    using SelfAlias = DListConstIterator<T>;
    using NodeAlias = DListNode<T>;
    using ValueTypeAlias = T;
    using PointerAlias = const T*;
    using ReferenceAlias = const T&;

    DListConstIterator() noexcept : NodeBase()
    {
    }

    explicit DListConstIterator(const DListNodeBase* nodeBase) noexcept : NodeBase(nodeBase)
    {
    }

    DListConstIterator(const DListIterator<T>& iter) noexcept : NodeBase(iter.NodeBase)
    {
    }

    ReferenceAlias operator*() const noexcept
    {
        return *static_cast<const DListNode<T>*>(this->NodeBase)->GetData();
    }

    PointerAlias operator->() const noexcept
    {
        return static_cast<const DListNode<T>*>(this->NodeBase)->GetData();
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

    SelfAlias& operator--() noexcept
    {
        this->NodeBase = this->NodeBase->PointerPrev;
        return *this;
    }

    SelfAlias operator--(int) noexcept
    {
        SelfAlias temp(*this);
        this->NodeBase = this->NodeBase->PointerPrev;
        return temp;
    }

    friend bool operator!=(const DListConstIterator<T>& lhs, const DListConstIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase != rhs.NodeBase;
    }

    friend bool operator==(const DListConstIterator<T>& lhs, const DListConstIterator<T>& rhs) noexcept
    {
        return lhs.NodeBase == rhs.NodeBase;
    }

    SelfAlias PointerNext() const noexcept
    {
        if (this->NodeBase)
        {
            return SelfAlias(this->NodeBase->PointerNext);
        }
        else
        {
            return SelfAlias();
        }
    }

    SelfAlias PointerPrev() const noexcept
    {
        if (this->NodeBase)
        {
            return SelfAlias(this->NodeBase->PointerPrev);
        }
        else
        {
            return SelfAlias();
        }
    }
};

} // namespace DSALibraries::Containers
#endif // DSA_LIBRARIES_DLIST_ITERATOR_HPP