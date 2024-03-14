
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
    {
        CListNodeBase* tail = this->PointerNext;

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