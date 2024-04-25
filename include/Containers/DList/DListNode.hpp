#ifndef DSA_LIBRARIES_DLIST_NODE_HPP
#define DSA_LIBRARIES_DLIST_NODE_HPP
#include "../../Utilities/AlignedBuffer.hpp"
#include <cstddef>
#include <memory>
#include <utility>
namespace DSALibraries
{
namespace Containers
{
struct DListNodeBase
{
    DListNodeBase *PointerNext = nullptr;
    DListNodeBase *PointerPrev = nullptr;

    DListNodeBase() = default;

    DListNodeBase(DListNodeBase &&nodeBase) noexcept
        : PointerNext(nodeBase.PointerNext), PointerPrev(nodeBase.PointerPrev)
    {
        nodeBase.PointerNext = nullptr;
        nodeBase.PointerPrev = nullptr;
    }

    DListNodeBase &operator=(DListNodeBase &&nodeBase) noexcept
    {
        PointerNext = nodeBase.PointerNext;
        PointerPrev = nodeBase.PointerPrev;
        nodeBase.PointerNext = nullptr;
        nodeBase.PointerPrev = nullptr;
        return *this;
    }

    DListNodeBase(const DListNodeBase &) = delete;
    DListNodeBase &operator=(const DListNodeBase &) = delete;

    DListNodeBase *TransferAfter(DListNodeBase *begin, DListNodeBase *end) noexcept
    {
        DListNodeBase *const keep = begin->PointerNext;
        if (end)
        {
            begin->PointerNext = end->PointerNext;
            if (end->PointerNext)
            {
                end->PointerNext->PointerPrev = begin;
            }

            end->PointerNext = this->PointerNext;

            if (this->PointerNext)
            {
                this->PointerNext->PointerPrev = end;
            }
        }
        else
        {
            begin->PointerNext = nullptr;
        }

        this->PointerNext = keep;
        keep->PointerPrev = this;
        return end;
    }

    void ReverseAfter() noexcept
    {
        DListNodeBase *tail = this->PointerNext;

        if (!tail)
        {
            return;
        }

        while (DListNodeBase *tempNode = tail->PointerNext)
        {
            DListNodeBase *keepNode = this->PointerNext;
            this->PointerNext = tempNode;

            if (tempNode->PointerNext)
            {
                tempNode->PointerNext->PointerPrev = tail;
            }

            tail->PointerNext = tempNode->PointerNext;
            tempNode->PointerPrev = this;
            tempNode->PointerNext = keepNode;
            keepNode->PointerPrev = tempNode;
        }
    }

    void HookBeforeInternal(DListNodeBase *nodeBase) noexcept
    {
        this->PointerNext = nodeBase;
        this->PointerPrev = nodeBase->PointerPrev;
        nodeBase->PointerPrev->PointerNext = this;
        nodeBase->PointerPrev = this;
    }

    void HookAfterInternal(DListNodeBase *nodeBase) noexcept
    {
        this->PointerPrev = nodeBase;
        this->PointerNext = nodeBase->PointerNext;
        if (nodeBase->PointerNext)
        {
            nodeBase->PointerNext->PointerPrev = this;
        }
        nodeBase->PointerNext = this;
    }

    void UnhookInternal() noexcept
    {
        DListNodeBase *const nextNodeBase = this->PointerNext;
        DListNodeBase *const prevNodeBase = this->PointerPrev;
        if (nextNodeBase)
        {
            nextNodeBase->PointerPrev = prevNodeBase;
        }
        prevNodeBase->PointerNext = nextNodeBase;
    }
};

template <typename T> struct DListNode : public DListNodeBase
{
    DListNode() = default;

    Utilities::AlignedBuffer<T> data;

    T *GetData() noexcept
    {
        return data.Pointer();
    }

    const T *GetData() const noexcept
    {
        return data.Pointer();
    }
};

} // namespace Containers
} // namespace DSALibraries
#endif