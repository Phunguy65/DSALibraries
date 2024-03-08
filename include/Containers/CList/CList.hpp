#ifndef DSA_LIBRARIES_CLIST_HPP
#define DSA_LIBRARIES_CLIST_HPP
#include "../../Utilities/Allocator.hpp"
#include "CListIterator.hpp"
#include "CListNode.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

using namespace DSALibraries::Utilities;
namespace DSALibraries::Containers
{
template <typename T, typename Alloc> class CListBase
{
  protected:
    using TypeAllocatorTraitsAlias = std::allocator_traits<Alloc>;
    using NodeAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<CListNode<T>>;
    using TpAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<T>;

    struct CListCore : public NodeAllocatorTypeAlias
    {
        CListNodeBase NodeHead;

        CListCore() : NodeAllocatorTypeAlias(), NodeHead()
        {
            InitializeHead();
        }

        explicit CListCore(NodeAllocatorTypeAlias&& node) noexcept : NodeAllocatorTypeAlias(std::move(node)), NodeHead()
        {
            InitializeHead();
        }

        explicit CListCore(CListNodeBase&& head, NodeAllocatorTypeAlias&& node) noexcept
            : NodeHead(std::move(head)), NodeAllocatorTypeAlias(std::move(node))
        {
        }

        void InitializeHead() noexcept
        {
            this->NodeHead.PointerNext = &this->NodeHead;
        }
    };

  public:
    CListCore LinkedListCore;

    using NodeAlias = CListNode<T>;
    using NodeBaseAlias = CListNodeBase;
    using IteratorAlias = CListIterator<T>;
    using ConstIteratorAlias = CListConstIterator<T>;
    using AllocatorTypeAlias = Alloc;

    NodeAllocatorTypeAlias& GetNodeAllocator() noexcept
    {
        return *static_cast<NodeAllocatorTypeAlias*>(&this->LinkedListCore);
    }

    const NodeAllocatorTypeAlias& GetNodeAllocator() const noexcept
    {
        return *static_cast<const NodeAllocatorTypeAlias*>(&this->LinkedListCore);
    }

    TpAllocatorTypeAlias GetTAllocator() const noexcept
    {
        return TpAllocatorTypeAlias(GetNodeAllocator());
    }

    AllocatorTypeAlias GetAllocator() const noexcept
    {
        return AllocatorTypeAlias(this->GetNodeAllocator());
    }

  private:
  public:
    CListBase() = default;

    explicit CListBase(NodeAllocatorTypeAlias&& al) noexcept : LinkedListCore(std::move(al))
    {
    }

    CListBase(CListBase&& list, NodeAllocatorTypeAlias&& al, std::true_type) noexcept
        : LinkedListCore(std::move(list.LinkedListCore.NodeHead), std::move(al))
    {
    }

    CListBase(CListBase&& list, NodeAllocatorTypeAlias&& al) noexcept : LinkedListCore(std::move(al))
    {
        if (list.GetNodeAllocator() == this->GetNodeAllocator())
        {
            this->LinkedListCore.NodeHead = std::move(list.LinkedListCore.NodeHead);
        }
    }

    ~CListBase() noexcept
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead, &this->LinkedListCore.NodeHead);
    }

  protected:
    void JoinTwoNodes(NodeBaseAlias* first, NodeBaseAlias* second)
    {
        first->PointerNext = second;
    }
    template <typename... Args> NodeBaseAlias* InsertAfterInternal(ConstIteratorAlias pos, Args&&... args)
    {
        NodeBaseAlias* to = const_cast<NodeBaseAlias*>(pos.NodeBase);

        NodeAlias* thing = this->CreateNodeInternal(std::forward<Args>(args)...);

        thing->PointerNext = to->PointerNext;

        to->PointerNext = thing;

        return to->PointerNext;
    }

    NodeBaseAlias* EraseAfterInternal(NodeBaseAlias* pos)
    {
        NodeAlias* curr = static_cast<NodeAlias*>(pos->PointerNext);

        pos->PointerNext = curr->PointerNext;

        this->GetTAllocator().destroy(curr->GetData());

        curr->~NodeAlias();

        this->PutNode(curr);

        return pos->PointerNext;
    }
    NodeBaseAlias* EraseAfterInternal(NodeBaseAlias* pos, NodeBaseAlias* last)
    {
        NodeAlias* curr = static_cast<NodeAlias*>(pos->PointerNext);

        while (curr != last)
        {
            NodeAlias* temp = curr;

            curr = static_cast<NodeAlias*>(curr->PointerNext);

            this->GetTAllocator().destroy(temp->GetData());

            temp->~NodeAlias();

            this->PutNode(temp);
        }

        pos->PointerNext = last;
        return last;
    }

    NodeAlias* GetNode()
    {
        return static_cast<NodeAlias*>(this->LinkedListCore.NodeAllocatorTypeAlias::allocate(1));
    }

    void PutNode(CListNode<T>* node)
    {
        this->LinkedListCore.NodeAllocatorTypeAlias::deallocate(node, 1);
    }

    template <typename... Args> NodeAlias* CreateNodeInternal(Args&&... args)
    {
        NodeAlias* node = this->GetNode();
        try
        {
            this->GetTAllocator().construct(node->GetData(), std::forward<Args>(args)...);
            node->PointerNext = nullptr;
        }
        catch (...)
        {
            this->PutNode(node);
        }
        return node;
    }

    NodeAlias* CreateNodeInternal(const T& value)
    {
        NodeAlias* node = this->GetNode();
        try
        {
            this->GetTAllocator().construct(node->GetData(), value);
            node->PointerNext = nullptr;
        }
        catch (...)
        {
            this->PutNode(node);
        }
        return node;
    }
};

template <typename T, typename Alloc = Allocator<T>> class CList : private CListBase<T, Alloc>
{
  private:
    using CListBase = CListBase<T, Alloc>;
    using NodeBaseAlias = typename CListBase::NodeBaseAlias;
    using NodeAlias = typename CListBase::NodeAlias;

  public:
    using ValueTypeAlias = T;
    using ReferenceAlias = ValueTypeAlias&;
    using ConstReferenceAlias = const ValueTypeAlias&;
    using IteratorAlias = typename CListBase::IteratorAlias;
    using ConstIteratorAlias = typename CListBase::ConstIteratorAlias;
    using SizeTypeAlias = std::size_t;
    using AllocatorTypeAlias = typename CListBase::AllocatorTypeAlias;

    using NodeAllocatorTypeAlias = typename CListBase::NodeAllocatorTypeAlias;

  private:
    CList(CList&& sll, NodeAllocatorTypeAlias&& al, std::true_type) : CListBase(std::move(sll), std::move(al))
    {
    }

    CList(CList&& sll, NodeAllocatorTypeAlias&& al, std::false_type) : CListBase(std::move(sll), std::move(al))
    {
        this->InsertAfterInternal(&this->LinkedListCore.NodeHead, sll.GetBegin(), sll.GetEnd());
    }

  public:
    CList() noexcept = default;

    explicit CList(const AllocatorTypeAlias& al) noexcept : CListBase(NodeAllocatorTypeAlias(al))
    {
    }

    explicit CList(SizeTypeAlias size, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : CListBase(AllocatorTypeAlias(al))
    {
        DefaultInitializeInternal(size);
    }

    CList(SizeTypeAlias size, const T& value, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : CListBase(NodeAllocatorTypeAlias(al))
    {
        FillInitializeInternal(size, value);
    }

    CList(const CList& sll) : CListBase(NodeAllocatorTypeAlias::select_on_container_copy_construction())
    {
        RangeInitializeInternal(sll.GetBegin(), sll.GetConstEnd());
    }

    CList(CList&& sll, const AllocatorTypeAlias& al)
        : CList(std::move(sll), NodeAllocatorTypeAlias(al), typename NodeAllocatorTypeAlias::is_always_equal())
    {
    }

    template <typename InputIterator>
    CList(InputIterator first, InputIterator last, const Alloc& al = Alloc()) : CListBase(NodeAllocatorTypeAlias(al))
    {
        RangeInitializeInternal(first, last);
    }

    CList(std::initializer_list<T> il, const Alloc& al = Alloc()) : CListBase(NodeAllocatorTypeAlias(al))
    {
        RangeInitializeInternal(il.begin(), il.end());
    }

    ~CList() noexcept = default;

    CList& operator=(const CList& list)
    {
        if (this != &list)
        {
            // Clear();
            IteratorAlias oldIteratorPrev = this->GetBeginFromHead();
            IteratorAlias oldIteratorCurr = this->GetBegin();
            IteratorAlias oldIteratorLast = this->GetEnd();

            ConstIteratorAlias newIteratorFirst = list.GetConstBegin();
            ConstIteratorAlias newIteratorLast = list.GetConstEnd();

            while (oldIteratorCurr != oldIteratorLast && newIteratorFirst != newIteratorLast)
            {
                *oldIteratorCurr = *newIteratorFirst;
                ++oldIteratorCurr;
                ++newIteratorFirst;
                ++oldIteratorPrev;
            }
            if (newIteratorFirst == newIteratorLast)
            {
                this->EraseAfter(oldIteratorPrev, oldIteratorLast);
            }
            else
            {
                this->InsertAfter(oldIteratorPrev, newIteratorFirst, newIteratorLast);
            }
        }
        return *this;
    }

    CList& operator=(CList&& sll) noexcept
    {
        this->Clear();
        this->Swap(sll);
        return *this;
    }

    IteratorAlias GetBegin()
    {
        return IteratorAlias(this->LinkedListCore.NodeHead.PointerNext);
    }

    ConstIteratorAlias GetConstBegin() const noexcept
    {
        return ConstIteratorAlias(this->LinkedListCore.NodeHead.PointerNext);
    }

    IteratorAlias GetEnd()
    {
        return IteratorAlias(&this->LinkedListCore.NodeHead);
    }

    ConstIteratorAlias GetConstEnd() const noexcept
    {
        return ConstIteratorAlias(&this->LinkedListCore.NodeHead);
    }

    // IteratorAlias GetBeginFromHead()
    // {
    //     return IteratorAlias(&this->LinkedListCore.NodeHead);
    // }

    // ConstIteratorAlias GetConstBeginFromHead() const noexcept
    // {
    //     return ConstIteratorAlias(&this->LinkedListCore.NodeHead);
    // }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return this->LinkedListCore.NodeHead.PointerNext == &this->LinkedListCore.NodeHead;
    }

    [[nodiscard]] SizeTypeAlias MaxSize() const noexcept
    {
        return this->GetNodeAllocator().max_size();
    }

    [[nodiscard]] SizeTypeAlias GetSize() const noexcept
    {
        SizeTypeAlias size = 0;
        for (ConstIteratorAlias iter = this->GetConstBegin(); iter != this->GetConstEnd(); ++iter)
        {
            ++size;
        }
        return size;
    }

    ReferenceAlias GetItemFront()
    {
        NodeAlias* front = static_cast<NodeAlias*>(this->LinkedListCore.NodeHead.PointerNext);
        return *front->GetData();
    }

    ConstReferenceAlias GetItemFront() const
    {
        NodeAlias* front = static_cast<NodeAlias*>(this->LinkedListCore.NodeHead.PointerNext);
        return *front->GetData();
    }

    void PushFront(const T& value)
    {
        this->InsertAfterInternal(this->GetConstEnd(), value);
    }

    void PushFront(T&& value)
    {
        this->InsertAfterInternal(this->GetConstEnd(), std::move(value));
    }

    void PopFront()
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead);
    }

    template <typename... Args> void EmplaceFront(Args&&... args)
    {
        this->InsertAfterInternal(GetConstEnd(), std::forward<Args>(args)...);
    }

    template <typename... Args> IteratorAlias EmplaceAfter(ConstIteratorAlias pos, Args&&... args)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, std::forward<Args>(args)...));
    }

    template <typename InputIterator> void Assign(InputIterator first, InputIterator last)
    {
        this->Clear();
        InsertAfter(GetConstEnd(), first, last);
    }

    void Assign(SizeTypeAlias size, const T& value)
    {
        this->Clear();
        InsertAfter(GetConstEnd(), size, value);
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, const T& value)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, value));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, T&& value)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, std::move(value)));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, SizeTypeAlias size, const T& value)
    {
        if (size)
        {
            CList temp(size, value, this->GetNodeAllocator());
            return this->SpliceAfterInternal(pos, ConstIteratorAlias(&this->LinkedListCore.NodeHead),
                                             temp.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<NodeBaseAlias*>(pos.NodeBase));
        }
    }

    template <typename InputIterator>
    IteratorAlias InsertAfter(ConstIteratorAlias pos, InputIterator first, InputIterator last)
    {
        CList temp(first, last, this->GetNodeAllocator());

        if (!temp.IsEmpty())
        {
            return this->SpliceAfterInternal(pos, ConstIteratorAlias(&this->LinkedListCore.NodeHead),
                                             temp.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<NodeBaseAlias*>(pos.NodeBase));
        }
    }

    void SpliceAfter(ConstIteratorAlias pos, CList&& sll)
    {
        if (!sll.IsEmpty())
        {
            this->SpliceAfterInternal(pos, ConstIteratorAlias(&sll.LinkedListCore.NodeHead), sll.GetConstEnd());
        }
    }

    void SpliceAfter(ConstIteratorAlias pos, CList&& sll, ConstIteratorAlias it)
    {
        ConstIteratorAlias next = it;
        ++next;

        if (pos == it || pos == next)
        {
            return;
        }

        NodeBaseAlias* current = const_cast<NodeBaseAlias*>(pos.NodeBase);
        current->TransferAfter(const_cast<NodeBaseAlias*>(it.NodeBase), const_cast<NodeBaseAlias*>(next.NodeBase));
    }

    void SpliceAfter(ConstIteratorAlias pos, CList&& sll, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        SpliceAfterInternal(pos, first, last);
    }

    void SpliceAfter(ConstIteratorAlias pos, CList& sll, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        this->SpliceAfter(pos, std::move(sll), first, last);
    }

    void SpliceAfter(ConstIteratorAlias pos, CList& sll)
    {
        this->SpliceAfter(pos, std::move(sll));
    }

    IteratorAlias EraseAfter(ConstIteratorAlias pos)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias*>(pos.NodeBase)));
    }

    IteratorAlias EraseAfter(ConstIteratorAlias pos, ConstIteratorAlias last)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias*>(pos.NodeBase)),
                             const_cast<NodeBaseAlias*>(last.NodeBase));
    }

    void Resize(SizeTypeAlias size)
    {
        IteratorAlias curr = &this->LinkedListCore.NodeHead;

        SizeTypeAlias length = 0;

        while (curr.PointerNext() != this->GetEnd() && length < size)
        {
            ++length;
            ++curr;
        }

        if (length == size)
        {
            this->EraseAfter(curr, this->GetEnd());
        }
        else
        {
            this->DefaultInsertAfter(curr, size - length);
        }
    }

    void Resize(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        IteratorAlias curr = &this->LinkedListCore.NodeHead;
        SizeTypeAlias length = 0;

        while (curr.Next() != this->GetEnd() && length < size)
        {
            ++length;
            ++curr;
        }

        if (length == size)
        {
            this->EraseAfter(curr, this->GetEnd());
        }
        else
        {
            this->InsertAfter(curr, size - length, value);
        }
    }

    void Clear() noexcept
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead, GetEnd().NodeBase);
    }

    void Remove(const ValueTypeAlias& value)
    {
        NodeBaseAlias* curr = &this->LinkedListCore.NodeHead;
        NodeBaseAlias* saveNode = nullptr;
        NodeAlias* temp = static_cast<NodeAlias*>(curr->PointerNext);

        while (temp != &this->LinkedListCore.NodeHead)
        {
            if (*temp->GetData() == value)
            {
                if (temp->GetData() != std::addressof(value))
                {
                    this->EraseAfterInternal(curr);
                    continue;
                }
                else
                {
                    saveNode = curr;
                }
            }
            curr = curr->PointerNext;
            temp = static_cast<NodeAlias*>(curr->PointerNext);
        }

        if (saveNode)
        {
            this->EraseAfterInternal(saveNode);
        }
    }

    template <typename Predicate> void RemoveIf(Predicate pred)
    {
        NodeBaseAlias* curr = &this->LinkedListCore.NodeHead;

        NodeAlias* temp = static_cast<NodeAlias*>(curr->PointerNext);

        while (temp != &this->LinkedListCore.NodeHead)
        {
            if (pred(*temp->GetData()))
            {
                this->EraseAfterInternal(curr);
            }
            else
            {
                curr = curr->PointerNext;
            }
            temp = static_cast<NodeAlias*>(curr->PointerNext);
        }
    }

    template <typename Compare> void Merge(CList&& sll, Compare comp)
    {
        if (std::addressof(sll) == this)
        {
            return;
        }

        NodeBaseAlias* node = static_cast<NodeBaseAlias*>(&this->LinkedListCore.NodeHead);

        while (node->PointerNext != &this->LinkedListCore.NodeHead &&
               sll.LinkedListCore.NodeHead.PointerNext != &sll.LinkedListCore.NodeHead)
        {
            if (comp(*static_cast<NodeAlias*>(sll.LinkedListCore.NodeHead.PointerNext)->GetData(),
                     *static_cast<NodeAlias*>(node->PointerNext)->GetData()))
            {
                node->TransferAfter(&sll.LinkedListCore.NodeHead, sll.LinkedListCore.NodeHead.PointerNext);
            }

            node = node->PointerNext;
        }

        if (sll.LinkedListCore.NodeHead.PointerNext != &sll.LinkedListCore.NodeHead)
        {
            *node = std::move(sll.LinkedListCore.NodeHead);
        }
    }

    template <typename Compare> void Merge(CList& sll, Compare comp)
    {
        this->Merge(std::move(sll), comp);
    }

    void Merge(CList&& sll)
    {
        this->Merge(std::move(sll), std::less<T>());
    }

    void Merge(CList& sll)
    {
        this->Merge(std::move(sll), std::less<T>());
    }

    template <typename Compare> void Sort(Compare compare)
    {
        // selection sort

        NodeBaseAlias* curr = &this->LinkedListCore.NodeHead;

        while (curr->PointerNext != &this->LinkedListCore.NodeHead)
        {
            NodeBaseAlias* min = curr;
            NodeBaseAlias* temp = curr->PointerNext;

            while (temp != &this->LinkedListCore.NodeHead)
            {
                if (compare(*static_cast<NodeAlias*>(temp)->GetData(), *static_cast<NodeAlias*>(min)->GetData()))
                {
                    min = temp;
                }
                temp = temp->PointerNext;
            }

            if (min != curr)
            {
                curr->TransferAfter(min, min->PointerNext);
            }
            else
            {
                curr = curr->PointerNext;
            }
        }
    }

    void Sort()
    {
        this->Sort(std::less<T>());
    }

    void Swap(CList& sll) noexcept
    {
        // swap circular single linked list
        if (std::addressof(sll) != this)
        {
            std::swap(this->LinkedListCore.NodeHead.PointerNext, sll.LinkedListCore.NodeHead.PointerNext);

            IteratorAlias oldTail = this->GetBegin();

            while (oldTail.PointerNext() != this->GetEnd())
            {
                ++oldTail;
            }

            IteratorAlias newTail = sll.GetBegin();

            while (newTail.PointerNext() != sll.GetEnd())
            {
                ++newTail;
            }

            NodeBaseAlias* oldTailNodeBase = static_cast<NodeBaseAlias*>(oldTail.NodeBase);
            NodeBaseAlias* newTailNodeBase = static_cast<NodeBaseAlias*>(newTail.NodeBase);

            sll.JoinTwoNodes(newTailNodeBase, &this->LinkedListCore.NodeHead);
            this->JoinTwoNodes(oldTailNodeBase, &sll.LinkedListCore.NodeHead);
        }
    }

    void Reverse() noexcept
    {
        this->LinkedListCore.NodeHead.ReverseAfter();
    }

  private:
    void FillInitializeInternal(SizeTypeAlias size, const T& value)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;
        while (size)
        {
            temp->PointerNext = this->CreateNodeInternal(value);
            temp = temp->PointerNext;
            --size;
        }
        this->JoinTwoNodes(temp, &this->LinkedListCore.NodeHead);
    }

    void DefaultInitializeInternal(SizeTypeAlias size)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;
        while (size)
        {
            temp->PointerNext = this->CreateNodeInternal();
            temp = temp->PointerNext;
            --size;
        }
        this->JoinTwoNodes(temp, &this->LinkedListCore.NodeHead);
    }

    void DefaultInsertAfterInternal(ConstIteratorAlias pos, SizeTypeAlias size)
    {
        ConstIteratorAlias savePos = pos;
        try
        {
            for (; size; --size)
            {
                pos = this->EmplaceAfter(pos);
            }
        }
        catch (...)
        {
            this->EraseAfter(savePos, pos);
            throw;
        }
    }

    IteratorAlias SpliceAfterInternal(ConstIteratorAlias pos, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        NodeBaseAlias* temp = const_cast<NodeBaseAlias*>(pos.NodeBase);

        NodeBaseAlias* nextOtherNodeBase = const_cast<NodeBaseAlias*>(first.NodeBase);

        NodeBaseAlias* end = nextOtherNodeBase;

        while (end && end->PointerNext != last.NodeBase)
        {
            end = end->PointerNext;
        }

        if (nextOtherNodeBase != end)
        {
            return IteratorAlias(temp->TransferAfter(nextOtherNodeBase, end));
        }
        else
        {
            return IteratorAlias(temp);
        }
    }

    template <typename InputIterator> void RangeInitializeInternal(InputIterator first, InputIterator last)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;
        while (first != last)
        {
            temp->PointerNext = this->CreateNodeInternal(*first);
            temp = temp->PointerNext;
            ++first;
        }
        this->JoinTwoNodes(temp, &this->LinkedListCore.NodeHead);
    }

    void AssignInternal(SizeTypeAlias size, const T& value, std::true_type)
    {
        IteratorAlias previous = &this->LinkedListCore.NodeHead;
        IteratorAlias current = this->GetBegin();
        IteratorAlias last = this->GetEnd();

        while (current != last && size)
        {
            *current = value;
            ++current;
            ++previous;
            --size;
        }

        if (size)
        {
            this->InsertAfter(previous, size, value);
        }
        else
        {
            this->EraseAfter(previous, last);
        }
    }

    void AssignInternal(SizeTypeAlias size, const T& value, std::false_type)
    {
        this->Clear();
        this->FillInitializeInternal(size, value);
    }
};

template <typename T, typename Alloc> bool operator==(const CList<T, Alloc>& lhs, const CList<T, Alloc>& rhs)
{
    if (lhs.GetSize() != rhs.GetSize())
    {
        return false;
    }
    auto lhsIter = lhs.GetConstBegin();
    auto rhsIter = rhs.GetConstBegin();
    while (lhsIter != lhs.GetConstEnd() && rhsIter != rhs.GetConstEnd())
    {
        if (*lhsIter != *rhsIter)
        {
            return false;
        }
        ++lhsIter;
        ++rhsIter;
    }
    return true;
}

template <typename T, typename Alloc> inline bool operator!=(const CList<T, Alloc>& lhs, const CList<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc> inline void Swap(CList<T, Alloc>& lhs, CList<T, Alloc>& rhs) noexcept
{
    lhs.Swap(rhs);
}
}; // namespace DSALibraries::Containers
#endif