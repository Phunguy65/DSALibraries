#include <bits/stdc++.h>
#include <cstddef>
#include <utility>
#ifndef DSA_LIBRARIES_LIST_NODE_HPP
#define DSA_LIBRARIES_LIST_NODE_HPP 1
namespace DSALibraries
{
namespace Containers
{
struct ListNodeBase
{
    ListNodeBase* PointerNext;
    ListNodeBase* PointerPrev;

    static void Swap(ListNodeBase& lhs, ListNodeBase& rhs) noexcept
    {
        if (lhs.PointerNext != &rhs)
        {
            if (rhs.PointerNext != &lhs)
            {
                std::swap(lhs.PointerNext, rhs.PointerNext);
                std::swap(lhs.PointerPrev, rhs.PointerPrev);
                lhs.PointerNext->PointerPrev = &lhs;
                lhs.PointerPrev->PointerNext = &lhs;
                rhs.PointerNext->PointerPrev = &rhs;
                rhs.PointerPrev->PointerNext = &rhs;
            }
            else
            {
                rhs.Swap(rhs, lhs);
            }
        }
        else
        {
            lhs.Swap(lhs, rhs);
        }
    }

    void TransferAfterInternal(ListNodeBase* begin, ListNodeBase* end) noexcept
    {
        if (this != end)
        {
            end->PointerPrev->PointerNext = this;
            begin->PointerPrev->PointerNext = end;
            this->PointerPrev->PointerNext = begin;

            ListNodeBase* const temp = this->PointerPrev;
            this->PointerPrev = end->PointerPrev;
            end->PointerPrev = begin->PointerPrev;
            begin->PointerPrev = temp;
        }
    }

    void ReverseAfterInternal() noexcept
    {
        ListNodeBase* temp = this;
        do
        {
            std::swap(temp->PointerNext, temp->PointerPrev);
            temp = temp->PointerPrev;
        } while (temp != this);
    }

    void HookInternal(ListNodeBase* nodeBase) noexcept
    {
        this->PointerNext = nodeBase;
        this->PointerPrev = nodeBase->PointerPrev;
        nodeBase->PointerPrev->PointerNext = this;
        nodeBase->PointerPrev = this;
    }

    void UnhookInternal() noexcept
    {
        ListNodeBase* const nextNodeBase = this->PointerNext;
        ListNodeBase* const prevNodeBase = this->PointerPrev;
        prevNodeBase->PointerNext = nextNodeBase;
        nextNodeBase->PointerPrev = prevNodeBase;
    }
};

template <typename T> struct ListNode : public ListNodeBase
{
    alignas(T) std::byte Data[sizeof(T)];

    ListNode() = default;

    T* GetData() noexcept
    {
        return static_cast<T*>(static_cast<void*>(&Data));
    }

    const T* GetData() const noexcept
    {
        return static_cast<const T*>(static_cast<const void*>(&Data));
    }
};

} // namespace Containers
} // namespace DSALibraries
#endif // LIST_NODE_HPP