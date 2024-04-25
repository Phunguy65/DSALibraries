
#ifndef DSA_LIBRARIES_SLIST_NODE_HPP
#define DSA_LIBRARIES_SLIST_NODE_HPP
#include "../../Utilities/AlignedBuffer.hpp"
#include <utility>
namespace DSALibraries::Containers
{
struct SListNodeBase
{
    SListNodeBase *PointerNext = nullptr;

    SListNodeBase() = default;

    SListNodeBase(SListNodeBase &&nodeBase) noexcept : PointerNext(nodeBase.PointerNext)
    {
        nodeBase.PointerNext = nullptr;
    }

    SListNodeBase(const SListNodeBase &) = delete;

    SListNodeBase &operator=(const SListNodeBase &) = delete;

    SListNodeBase &operator=(SListNodeBase &&nodeBase) noexcept
    {
        this->PointerNext = nodeBase.PointerNext;
        nodeBase.PointerNext = nullptr;
        return *this;
    }

    SListNodeBase *TransferAfter(SListNodeBase *begin, SListNodeBase *end) noexcept
    {
        SListNodeBase *keep = begin->PointerNext;
        if (end)
        {
            begin->PointerNext = end->PointerNext;
            end->PointerNext = this->PointerNext;
        }
        else
        {
            begin->PointerNext = nullptr;
        }
        PointerNext = keep;
        return end;
    }

    void ReverseAfter() noexcept
    { // Get the next node in the list
        SListNodeBase *tail = PointerNext;

        // If there is no next node, we're at the end of the list and there's nothing to reverse
        if (!tail)
        {
            return;
        }

        // While there are still nodes to process
        while (SListNodeBase *temp = tail->PointerNext)
        {
            // Keep a reference to the current next node
            SListNodeBase *keepNode = this->PointerNext;

            // Move the next node pointer to the node after the next node
            this->PointerNext = temp;

            // Update the next node's next pointer to skip the node after it
            tail->PointerNext = temp->PointerNext;

            // Point the new next node's next pointer to the old next node
            this->PointerNext->PointerNext = keepNode;
        }
    }
};

template <typename T> struct SListNode : public SListNodeBase
{
    SListNode() = default;

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
} // namespace DSALibraries::Containers
#endif // DSA_LIBRARIES_SLIST_NODE_HPP