#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP 1
#include "ListNode.hpp"
namespace DSALibraries
{
namespace Containers
{
template <typename T> struct ListIterator
{
    typedef ListIterator<T> SelfAlias;
    typedef ListNode<T> NodeAlias;

    typedef T ValueTypeAlias;
    typedef T* PointerTypeAlias;
    typedef T& ReferenceTypeAlias;

    ListNodeBase* NodeBase;

    ListIterator() : NodeBase()
    {
    }

    explicit ListIterator(ListNodeBase* nodeBase) : NodeBase(nodeBase)
    {
    }

    ReferenceTypeAlias operator*() const noexcept
    {
        return *static_cast<NodeAlias*>(NodeBase)->GetData();
    }

    PointerTypeAlias operator->() const noexcept
    {
        return static_cast<NodeAlias*>(NodeBase)->GetData();
    }

    SelfAlias& operator++() noexcept
    {
        NodeBase = NodeBase->PointerNext;
        return *this;
    }

    SelfAlias operator++(int) noexcept
    {
        SelfAlias temp = *this;
        NodeBase = NodeBase->PointerNext;
        return temp;
    }

    SelfAlias& operator--() noexcept
    {
        NodeBase = NodeBase->PointerPrev;
        return *this;
    }

    SelfAlias operator--(int) noexcept
    {
        SelfAlias temp = *this;
        NodeBase = NodeBase->PointerPrev;
        return temp;
    }

    bool operator==(const SelfAlias& rhs) const noexcept
    {
        return NodeBase == rhs.NodeBase;
    }

    bool operator!=(const SelfAlias& rhs) const noexcept
    {
        return NodeBase != rhs.NodeBase;
    }
};

template <typename T> struct ListConstIterator
{
    using SelfAlias = ListConstIterator<T>;
    using NodeAlias = ListNode<T>;

    using ValueTypeAlias = T;
    using PointerTypeAlias = const T*;
    using ReferenceTypeAlias = const T&;

    const ListNodeBase* NodeBase;

    ListConstIterator() : NodeBase()
    {
    }

    explicit ListConstIterator(const ListNodeBase* nodeBase) : NodeBase(nodeBase)
    {
    }

    ListConstIterator(const ListIterator<T>& iterator) : NodeBase(iterator.NodeBase)
    {
    }

    ReferenceTypeAlias operator*() const noexcept
    {
        return *static_cast<const NodeAlias*>(NodeBase)->GetData();
    }

    PointerTypeAlias operator->() const noexcept
    {
        return static_cast<const NodeAlias*>(NodeBase)->GetData();
    }

    SelfAlias& operator++() noexcept
    {
        NodeBase = NodeBase->PointerNext;
        return *this;
    }

    SelfAlias operator++(int) noexcept
    {
        SelfAlias temp = *this;
        NodeBase = NodeBase->PointerNext;
        return temp;
    }

    SelfAlias& operator--() noexcept
    {
        NodeBase = NodeBase->PointerPrev;
        return *this;
    }

    SelfAlias operator--(int) noexcept
    {
        SelfAlias temp = *this;
        NodeBase = NodeBase->PointerPrev;
        return temp;
    }

    bool operator==(const SelfAlias& rhs) const noexcept
    {
        return NodeBase == rhs.NodeBase;
    }

    bool operator!=(const SelfAlias& rhs) const noexcept
    {
        return NodeBase != rhs.NodeBase;
    }
};

template <typename T> inline bool operator==(const ListIterator<T>& lhs, const ListConstIterator<T>& rhs) noexcept
{
    return lhs.NodeBase == rhs.NodeBase;
}

template <typename T> inline bool operator!=(const ListIterator<T>& lhs, const ListConstIterator<T>& rhs) noexcept
{
    return lhs.NodeBase != rhs.NodeBase;
}

} // namespace Containers
}; // namespace DSALibraries

#endif