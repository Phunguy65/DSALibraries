
#ifndef DSA_LIBRARIES_CLIST_NODE_HPP
#define DSA_LIBRARIES_CLIST_NODE_HPP
#include <utility>
namespace DSALibraries::Containers
{
struct CListNodeBase
{
    CListNodeBase* PointerNext = nullptr;

    CListNodeBase() = default;

    CListNodeBase(CListNodeBase&& nodeBase) noexcept : PointerNext(nodeBase.PointerNext)
    {
        while (CListNodeBase* temp = nodeBase.PointerNext)
        {
            if (temp->PointerNext == &nodeBase)
            {
                temp->PointerNext = this;
                break;
            }
        }
        nodeBase.PointerNext = &nodeBase;
    }

    CListNodeBase(const CListNodeBase&) = delete;

    CListNodeBase& operator=(const CListNodeBase&) = delete;

    CListNodeBase& operator=(CListNodeBase&& nodeBase) noexcept
    {
        this->PointerNext = nodeBase.PointerNext;

        while (CListNodeBase* temp = nodeBase.PointerNext)
        {
            if (temp->PointerNext == &nodeBase)
            {
                temp->PointerNext = this;
                break;
            }
        }
        nodeBase.PointerNext = &nodeBase;
        return *this;
    }

    CListNodeBase* TransferAfter(CListNodeBase* begin, CListNodeBase* end) noexcept
    {
        CListNodeBase* keep = begin->PointerNext;

        begin->PointerNext = end->PointerNext;
        end->PointerNext = PointerNext;

        PointerNext = keep;
        return end;
    }

    void ReverseAfter() noexcept
    { // Get the next node in the list
        CListNodeBase* tail = this->PointerNext;

        // // If there is no next node, we're at the end of the list and there's nothing to reverse
        // if (!tail)
        // {
        //     return;
        // }

        // // While there are still nodes to process
        // while (CListNodeBase* temp = tail->PointerNext)
        // {
        //     // Keep a reference to the current next node
        //     CListNodeBase* keepNode = this->PointerNext;

        //     // Move the next node pointer to the node after the next node
        //     this->PointerNext = temp;

        //     // Update the next node's next pointer to skip the node after it
        //     tail->PointerNext = temp->PointerNext;

        //     // Point the new next node's next pointer to the old next node
        //     this->PointerNext->PointerNext = keepNode;
        // }

        if (tail == this)
        {
            return;
        }

        CListNodeBase* temp = tail->PointerNext;

        while (temp != this)
        {
            CListNodeBase* keepNode = this->PointerNext;
            this->PointerNext = temp;
            tail->PointerNext = temp->PointerNext;
            this->PointerNext->PointerNext = keepNode;

            temp = tail->PointerNext;
        }

        return;
    }
};

template <typename T> struct CListNode : public CListNodeBase
{
    alignas(T) std::byte data[sizeof(T)];

    CListNode() = default;

    T* GetData() noexcept
    {
        return static_cast<T*>(static_cast<void*>(&this->data));
    }

    const T* GetData() const noexcept
    {
        return static_cast<const T*>(static_cast<const void*>(&this->data));
    }
};
} // namespace DSALibraries::Containers
#endif // LINKEDLISTNODE_HPP