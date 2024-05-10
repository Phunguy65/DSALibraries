#ifndef DSA_LIBRARIES_SLIST_HPP
#define DSA_LIBRARIES_SLIST_HPP
#include "../../Utilities/Allocator.hpp"
#include "SListIterator.hpp"
#include "SListNode.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

using namespace DSALibraries::Utilities;
namespace DSALibraries::Containers
{
template <typename T, typename Alloc> class SListBase
{
  protected:
    using TypeAllocatorTraitsAlias = std::allocator_traits<Alloc>;
    using NodeAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<SListNode<T>>;
    using TpAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<T>;

    struct SListCore : public NodeAllocatorTypeAlias
    {
        SListNodeBase NodeHead;

        SListCore() : NodeAllocatorTypeAlias(), NodeHead()
        {
        }

        explicit SListCore(NodeAllocatorTypeAlias &&node) noexcept : NodeAllocatorTypeAlias(std::move(node)), NodeHead()
        {
        }

        explicit SListCore(SListNodeBase &&head, NodeAllocatorTypeAlias &&node) noexcept
            : NodeHead(std::move(head)), NodeAllocatorTypeAlias(std::move(node))
        {
        }
    };

  public:
    SListCore LinkedListCore;

    using NodeAlias = SListNode<T>;
    using NodeBaseAlias = SListNodeBase;
    using IteratorAlias = SListIterator<T>;
    using ConstIteratorAlias = SListConstIterator<T>;
    using AllocatorTypeAlias = Alloc;

    NodeAllocatorTypeAlias &GetNodeAllocator() noexcept
    {
        return *static_cast<NodeAllocatorTypeAlias *>(&this->LinkedListCore);
    }

    const NodeAllocatorTypeAlias &GetNodeAllocator() const noexcept
    {
        return *static_cast<const NodeAllocatorTypeAlias *>(&this->LinkedListCore);
    }

    TpAllocatorTypeAlias GetTpAllocator() const noexcept
    {
        return TpAllocatorTypeAlias(GetNodeAllocator());
    }

    AllocatorTypeAlias GetAllocator() const noexcept
    {
        return AllocatorTypeAlias(this->GetNodeAllocator());
    }

    SListBase() = default;

    explicit SListBase(NodeAllocatorTypeAlias &&al) noexcept : LinkedListCore(std::move(al))
    {
    }

    SListBase(SListBase &&list, NodeAllocatorTypeAlias &&al, std::true_type) noexcept
        : LinkedListCore(std::move(list.LinkedListCore.NodeHead), std::move(al))
    {
    }

    SListBase(SListBase &&list, NodeAllocatorTypeAlias &&al) noexcept : LinkedListCore(std::move(al))
    {
        if (list.GetNodeAllocator() == this->GetNodeAllocator())
        {
            this->LinkedListCore.NodeHead = std::move(list.LinkedListCore.NodeHead);
        }
    }

    ~SListBase() noexcept
    {
        EraseAfterInternal(&LinkedListCore.NodeHead, nullptr);
    }

  protected:
    template <typename... Args> NodeBaseAlias *InsertAfterInternal(ConstIteratorAlias pos, Args &&...args)
    {
        auto *curr = const_cast<NodeBaseAlias *>(pos.NodeBase);

        NodeAlias *newNode = this->CreateNodeInternal(std::forward<Args>(args)...);

        newNode->PointerNext = curr->PointerNext;

        curr->PointerNext = newNode;

        return curr->PointerNext;
    }

    NodeBaseAlias *EraseAfterInternal(NodeBaseAlias *pos)
    {
        auto *curr = static_cast<NodeAlias *>(pos->PointerNext);

        pos->PointerNext = curr->PointerNext;

        this->GetTpAllocator().destroy(curr->GetData());

        curr->~NodeAlias();

        this->PutNode(curr);

        return pos->PointerNext;
    }
    NodeBaseAlias *EraseAfterInternal(NodeBaseAlias *pos, NodeBaseAlias *last)
    {
        auto *curr = static_cast<NodeAlias *>(pos->PointerNext);

        while (curr != last)
        {
            NodeAlias *temp = curr;

            curr = static_cast<NodeAlias *>(curr->PointerNext);

            this->GetTpAllocator().destroy(temp->GetData());

            temp->~NodeAlias();

            this->PutNode(temp);
        }

        pos->PointerNext = last;
        return last;
    }

    NodeAlias *GetNode()
    {
        return static_cast<NodeAlias *>(this->LinkedListCore.NodeAllocatorTypeAlias::allocate(1));
    }

    void PutNode(SListNode<T> *node)
    {
        this->LinkedListCore.NodeAllocatorTypeAlias::deallocate(node, 1);
    }

    template <typename... Args> NodeAlias *CreateNodeInternal(Args &&...args)
    {
        NodeAlias *node = this->GetNode();
        try
        {
            this->GetTpAllocator().construct(node->GetData(), std::forward<Args>(args)...);
            node->PointerNext = nullptr;
        }
        catch (...)
        {
            this->PutNode(node);
        }
        return node;
    }

    NodeAlias *CreateNodeInternal(const T &value)
    {
        NodeAlias *node = this->GetNode();
        try
        {
            this->GetTpAllocator().construct(node->GetData(), value);
            node->PointerNext = nullptr;
        }
        catch (...)
        {
            this->PutNode(node);
        }
        return node;
    }
};

template <typename T, typename Alloc = Allocator<T>> class SList : protected SListBase<T, Alloc>
{
  private:
    using SListBaseAlias = SListBase<T, Alloc>;
    using NodeBaseAlias = typename SListBaseAlias::NodeBaseAlias;
    using NodeAlias = typename SListBaseAlias::NodeAlias;

  public:
    using ValueTypeAlias = T;
    using ReferenceAlias = ValueTypeAlias &;
    using ConstReferenceAlias = const ValueTypeAlias &;
    using IteratorAlias = typename SListBaseAlias::IteratorAlias;
    using ConstIteratorAlias = typename SListBaseAlias::ConstIteratorAlias;
    using SizeTypeAlias = std::size_t;
    using AllocatorTypeAlias = typename SListBaseAlias::AllocatorTypeAlias;
    using NodeAllocatorTypeAlias = typename SListBaseAlias::NodeAllocatorTypeAlias;

    // Adaptive alias

    using value_type [[maybe_unused]] = ValueTypeAlias;
    using reference [[maybe_unused]] = ReferenceAlias;
    using const_reference [[maybe_unused]] = ConstReferenceAlias;
    using iterator [[maybe_unused]] = IteratorAlias;
    using const_iterator [[maybe_unused]] = ConstIteratorAlias;
    using size_type [[maybe_unused]] = SizeTypeAlias;
    using allocator_type [[maybe_unused]] = AllocatorTypeAlias;

  private:
    SList(SList &&sll, NodeAllocatorTypeAlias &&al, std::true_type) : SListBaseAlias(std::move(sll), std::move(al))
    {
    }

    SList(SList &&sll, NodeAllocatorTypeAlias &&al, std::false_type) : SListBaseAlias(std::move(sll), std::move(al))
    {
        this->InsertAfterInternal(this->GetConstBeginFromHead(), sll.GetBegin(), sll.GetEnd());
    }

  public:
    SList() noexcept = default;

    explicit SList(const AllocatorTypeAlias &al) noexcept : SListBaseAlias(NodeAllocatorTypeAlias(al))
    {
    }

    explicit SList(SizeTypeAlias size, const AllocatorTypeAlias &al = AllocatorTypeAlias())
        : SListBaseAlias(AllocatorTypeAlias(al))
    {
        DefaultInitializeInternal(size);
    }

    SList(SizeTypeAlias size, const T &value, const AllocatorTypeAlias &al = AllocatorTypeAlias())
        : SListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        FillInitializeInternal(size, value);
    }

    SList(const SList &sll) : SListBaseAlias(NodeAllocatorTypeAlias::select_on_container_copy_construction())
    {
        RangeInitializeInternal(sll.GetBegin(), sll.GetConstEnd());
    }

    SList(SList &&sll, const AllocatorTypeAlias &al)
        : SList(std::move(sll), NodeAllocatorTypeAlias(al), typename NodeAllocatorTypeAlias::is_always_equal())
    {
    }

    template <typename InputIterator>
    SList(InputIterator first, InputIterator last, const Alloc &al = Alloc())
        : SListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitializeInternal(first, last);
    }

    SList(std::initializer_list<T> il, const Alloc &al = Alloc()) : SListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitializeInternal(il.begin(), il.end());
    }

    ~SList() noexcept = default;

    SList &operator=(const SList &list)
    {
        if (this != std::addressof(list))
        {
            if (this->GetNodeAllocator() == list.GetNodeAllocator())
            {
                this->AssignInternal(list.GetConstBegin(), list.GetConstEnd(), std::is_copy_assignable<T>());
            }
            else
            {
                this->AssignInternal(list.GetConstBegin(), list.GetConstEnd(), std::false_type());
            }
        }
        return *this;
    }

    SList &operator=(SList &&sll) noexcept
    {
        this->Clear();
        this->MoveAssignInternal(std::move(sll), std::is_nothrow_move_assignable<AllocatorTypeAlias>());
        return *this;
    }

    // Start Zone Adaptive Iterator
    IteratorAlias begin() noexcept
    {
        return this->GetBegin();
    }

    ConstIteratorAlias begin() const noexcept
    {
        return this->GetConstBegin();
    }

    ConstIteratorAlias cbegin() const noexcept
    {
        return this->GetConstBegin();
    }

    IteratorAlias end() noexcept
    {
        return this->GetEnd();
    }

    ConstIteratorAlias end() const noexcept
    {
        return this->GetConstEnd();
    }

    [[maybe_unused]] ConstIteratorAlias cend() const noexcept
    {
        return this->GetConstEnd();
    }

    IteratorAlias before_begin() noexcept
    {
        return this->GetBeginFromHead();
    }

    ConstIteratorAlias before_begin() const noexcept
    {
        return this->GetConstBeginFromHead();
    }

    ConstIteratorAlias cbefore_begin() const noexcept
    {
        return this->GetConstBeginFromHead();
    }

    // End Zone Adaptive Iterator

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
        return IteratorAlias(nullptr);
    }

    ConstIteratorAlias GetConstEnd() const noexcept
    {
        return ConstIteratorAlias(nullptr);
    }

    IteratorAlias GetBeginFromHead()
    {
        return IteratorAlias(&this->LinkedListCore.NodeHead);
    }

    ConstIteratorAlias GetConstBeginFromHead() const noexcept
    {
        return ConstIteratorAlias(&this->LinkedListCore.NodeHead);
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return this->LinkedListCore.NodeHead.PointerNext == nullptr;
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
        auto *front = static_cast<NodeAlias *>(this->LinkedListCore.NodeHead.PointerNext);
        return *front->GetData();
    }

    ConstReferenceAlias GetItemFront() const
    {
        auto *front = static_cast<NodeAlias *>(this->LinkedListCore.NodeHead.PointerNext);
        return *front->GetData();
    }

    void PushFront(const T &value)
    {
        this->InsertAfterInternal(this->GetConstBeginFromHead(), value);
    }

    void PushFront(T &&value)
    {
        this->InsertAfterInternal(this->GetConstBeginFromHead(), std::move(value));
    }

    void PopFront()
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead);
    }

    template <typename... Args> void EmplaceFront(Args &&...args)
    {
        this->InsertAfterInternal(this->GetBeginFromHead(), std::forward<Args>(args)...);
    }

    template <typename... Args> IteratorAlias EmplaceAfter(ConstIteratorAlias pos, Args &&...args)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, std::forward<Args>(args)...));
    }

    void Assign(SizeTypeAlias size, const T &value)
    {
        this->Clear();
        this->AssignInternal(size, value, std::is_copy_assignable<T>());
    }

    template <typename InputIterator> void Assign(InputIterator first, InputIterator last)
    {
        this->Clear();
        this->AssignInternal(first, last, std::is_copy_assignable<T>());
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, const T &value)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, value));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, T &&value)
    {
        return IteratorAlias(this->InsertAfterInternal(pos, std::move(value)));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias pos, SizeTypeAlias size, const T &value)
    {
        if (size)
        {
            SList temp(size, value, this->GetNodeAllocator());
            return this->SpliceAfterInternal(pos, temp.GetBeginFromHead(), temp.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<NodeBaseAlias *>(pos.NodeBase));
        }
    }

    template <typename InputIterator>
    IteratorAlias InsertAfter(ConstIteratorAlias pos, InputIterator first, InputIterator last)
    {
        SList temp(first, last, this->GetNodeAllocator());

        if (!temp.IsEmpty())
        {
            return this->SpliceAfterInternal(pos, temp.GetBeginFromHead(), temp.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<NodeBaseAlias *>(pos.NodeBase));
        }
    }

    void SpliceAfter(ConstIteratorAlias pos, SList &&sll)
    {
        if (!sll.IsEmpty())
        {
            this->SpliceAfterInternal(pos, sll.GetBeginFromHead(), sll.GetEnd());
        }
    }

    void SpliceAfter(ConstIteratorAlias pos, SList &&sll, ConstIteratorAlias it)
    {
        ConstIteratorAlias next = it;
        ++next;

        if (pos == it || pos == next)
        {
            return;
        }

        auto *current = const_cast<NodeBaseAlias *>(pos.NodeBase);
        current->TransferAfter(const_cast<NodeBaseAlias *>(it.NodeBase), const_cast<NodeBaseAlias *>(next.NodeBase));
    }

    void SpliceAfter(ConstIteratorAlias pos, SList &&sll, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        SpliceAfterInternal(pos, first, last);
    }

    void SpliceAfter(ConstIteratorAlias pos, SList &sll, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        this->SpliceAfter(pos, std::move(sll), first, last);
    }

    void SpliceAfter(ConstIteratorAlias pos, SList &sll)
    {
        this->SpliceAfter(pos, std::move(sll));
    }

    IteratorAlias EraseAfter(ConstIteratorAlias pos)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias *>(pos.NodeBase)));
    }

    IteratorAlias EraseAfter(ConstIteratorAlias pos, ConstIteratorAlias last)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias *>(pos.NodeBase)),
                             const_cast<NodeBaseAlias *>(last.NodeBase));
    }

    void Resize(SizeTypeAlias size)
    {
        IteratorAlias curr = this->GetBeginFromHead();

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
            this->DefaultInsertAfterInternal(curr, size - length);
        }
    }

    void Resize(SizeTypeAlias size, const ValueTypeAlias &value)
    {
        IteratorAlias curr = this->GetBeginFromHead();

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
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead, nullptr);
    }

    void Remove(const ValueTypeAlias &value)
    {
        NodeBaseAlias *curr = &this->LinkedListCore.NodeHead;
        NodeBaseAlias *saveNode = nullptr;

        while (auto *temp = static_cast<NodeAlias *>(curr->PointerNext))
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
        }

        if (saveNode)
        {
            this->EraseAfterInternal(saveNode);
        }
    }

    template <typename Predicate> void RemoveIf(Predicate pred)
    {
        NodeBaseAlias *curr = &this->LinkedListCore.NodeHead;

        while (auto *temp = static_cast<NodeAlias *>(curr->PointerNext))
        {
            if (pred(*temp->GetData()))
            {
                this->EraseAfterInternal(curr);
            }
            else
            {
                curr = curr->PointerNext;
            }
        }
    }

    template <typename Compare> void Merge(SList &&sll, Compare comp)
    {
        if (std::addressof(sll) == this)
        {
            return;
        }

        auto *node = static_cast<NodeBaseAlias *>(&this->LinkedListCore.NodeHead);

        while (node->PointerNext && sll.LinkedListCore.NodeHead.PointerNext)
        {
            if (compare(*static_cast<NodeAlias *>(sll.LinkedListCore.NodeHead.PointerNext)->GetData(),
                        *static_cast<NodeAlias *>(node->PointerNext)->GetData()))
            {
                node->TransferAfter(&sll.LinkedListCore.NodeHead, sll.LinkedListCore.NodeHead.PointerNext);
            }

            node = node->PointerNext;
        }

        if (sll.LinkedListCore.NodeHead.PointerNext)
        {
            *node = std::move(sll.LinkedListCore.NodeHead);
        }
    }

    template <typename Compare> void Merge(SList &sll, Compare comp)
    {
        this->Merge(std::move(sll), comp);
    }

    void Merge(SList &&sll)
    {
        this->Merge(std::move(sll), std::less<T>());
    }

    void Merge(SList &sll)
    {
        this->Merge(std::move(sll), std::less<T>());
    }

    template <typename TBinaryPredicate> SizeTypeAlias Unique(TBinaryPredicate predicate)
    {
        IteratorAlias first = this->GetBegin();
        IteratorAlias last = this->GetEnd();

        if (first == last)
        {
            return 0;
        }

        SList toDestroy(this->GetAllocator());
        SizeTypeAlias removed [[maybe_unused]] = 0;
        IteratorAlias next = first;

        while (++next != last)
        {
            if (predicate(*first, *next))
            {
                toDestroy.SpliceAfter(toDestroy.GetConstBeginFromHead(), *this, first);
                ++removed;
            }
            else
            {
                first = next;
            }
            next = first;
        }

        return removed;
    }

    SizeTypeAlias Unique()
    {
        return this->Unique(std::equal_to<T>());
    }
    template <typename TCompare> void Sort(TCompare compare)
    {
        // This is a merge sort implementation.

        IteratorAlias pLeft;
        IteratorAlias pRight;
        IteratorAlias pNode;
        IteratorAlias pHead;
        IteratorAlias pTail;

        SizeTypeAlias listSize = 1;
        SizeTypeAlias numberOfMerges;
        SizeTypeAlias leftSize;
        SizeTypeAlias rightSize;

        // If the list is empty or has only one element, it is already sorted.
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        while (true)
        {
            pLeft = this->GetBegin();
            pHead = this->GetBeginFromHead();
            pTail = this->GetBeginFromHead();

            numberOfMerges = 0;

            while (pLeft != this->GetEnd())
            {
                ++numberOfMerges;
                pRight = pLeft;
                leftSize = 0;

                for (SizeTypeAlias i = 0; i < listSize; ++i)
                {
                    ++leftSize;
                    ++pRight;
                    if (pRight == this->GetEnd())
                    {
                        break;
                    }
                }

                rightSize = listSize;

                while (leftSize > 0 || (rightSize > 0 && pRight != this->GetEnd()))
                {
                    if (leftSize == 0)
                    {
                        pNode = pRight;
                        ++pRight;
                        --rightSize;
                    }
                    else if (rightSize == 0 || pRight == this->GetEnd())
                    {
                        pNode = pLeft;
                        ++pLeft;
                        --leftSize;
                    }
                    else if (!compare(*pRight, *pLeft))
                    {
                        pNode = pLeft;
                        ++pLeft;
                        --leftSize;
                    }
                    else
                    {
                        pNode = pRight;
                        ++pRight;
                        --rightSize;
                    }

                    if (pHead == this->GetBeginFromHead())
                    {
                        pHead.NodeBase->PointerNext = pNode.NodeBase;
                        pHead = pNode;
                        pTail = pNode;
                    }
                    else
                    {
                        pTail.NodeBase->PointerNext = pNode.NodeBase;
                        pTail = pNode;
                    }
                    pTail.NodeBase->PointerNext = nullptr;
                }
                pLeft = pRight;
            }

            if (numberOfMerges <= 1)
            {
                return;
            }

            listSize *= 2;
        }
    }

    void Sort()
    {
        this->Sort(std::less<T>());
    }

    // Zone of DSA
    template <typename TCompare> void DSASort(unsigned choice, TCompare compare)
    {
        switch (choice)
        {
        case 1:
            this->InsertionSort(compare);
            break;
        case 2:
            this->SelectionSort(compare);
            break;
        case 3:
            this->BubbleSort(compare);
            break;
        case 4:
            this->QuickSort(compare);
            break;
        case 5:
            this->HeapSort(compare);
            break;
        default:
            this->Sort(compare);
            break;
        }
    }

    void Swap(SList &sll) noexcept
    {
        std::swap(this->LinkedListCore.NodeHead.PointerNext, sll.LinkedListCore.NodeHead.PointerNext);
        if (NodeAllocatorTypeAlias::propagate_on_container_swap::value)
        {
            std::swap(this->GetNodeAllocator(), sll.GetNodeAllocator());
        }
    }

    void Reverse() noexcept
    {
        this->LinkedListCore.NodeHead.ReverseAfter();
    }

  private:
    void FillInitializeInternal(SizeTypeAlias size, const T &value)
    {
        NodeBaseAlias *temp = &this->LinkedListCore.NodeHead;
        while (size)
        {
            temp->PointerNext = this->CreateNodeInternal(value);
            temp = temp->PointerNext;
            --size;
        }
    }

    void DefaultInitializeInternal(SizeTypeAlias size)
    {
        NodeBaseAlias *temp = &this->LinkedListCore.NodeHead;
        while (size)
        {
            temp->PointerNext = this->CreateNodeInternal();
            temp = temp->PointerNext;
            --size;
        }
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

    void AssignInternal(SizeTypeAlias size, const T &value, std::true_type)
    {
        IteratorAlias *prev = this->GetBeginFromHead();
        IteratorAlias *curr = this->GetBegin();
        IteratorAlias *last = this->GetEnd();

        while (curr != last && size)
        {
            *curr = value;
            ++curr;
            ++prev;
            --size;
        }

        if (size)
        {
            this->InsertAfter(prev, size, value);
        }
        else if (curr != last)
        {
            this->EraseAfter(prev, last);
        }
    }

    void AssignInternal(SizeTypeAlias size, const T &value, std::false_type)
    {
        this->Clear();
        this->FillInitializeInternal(size, value);
    }

    IteratorAlias SpliceAfterInternal(ConstIteratorAlias pos, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        NodeBaseAlias *temp = const_cast<NodeBaseAlias *>(pos.NodeBase);

        NodeBaseAlias *nextOtherNodeBase = const_cast<NodeBaseAlias *>(first.NodeBase);

        NodeBaseAlias *end = nextOtherNodeBase;

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
        NodeBaseAlias *temp = &this->LinkedListCore.NodeHead;
        while (first != last)
        {
            temp->PointerNext = this->CreateNodeInternal(*first);
            temp = temp->PointerNext;
            ++first;
        }
    }

    void MoveAssignInternal(SList &&sll, std::true_type)
    {
        this->Clear();
        this->GetNodeAllocator() = std::move(sll.GetNodeAllocator());
        this->LinkedListCore.NodeHead = std::move(sll.LinkedListCore.NodeHead);
    }

    void MoveAssignInternal(SList &&sll, std::false_type)
    {
        if (this->GetNodeAllocator() == sll.GetNodeAllocator())
        {
            MoveAssignInternal(std::move(sll), std::true_type());
        }
        else
        {
            this->Clear();
            this->AssignInternal(sll.GetConstBegin(), sll.GetConstEnd(), std::false_type());
        }
    }

    template <typename InputIterator> void AssignInternal(InputIterator first, InputIterator last, std::false_type)
    {
        this->Clear();
        this->InsertAfter(this->GetConstBeginFromHead(), first, last);
    }

    template <typename InputIterator> void AssignInternal(InputIterator first, InputIterator last, std::true_type)
    {
        IteratorAlias *prev = this->GetBeginFromHead();
        IteratorAlias *curr = this->GetBegin();
        IteratorAlias *end = this->GetEnd();

        while (curr != end && first != last)
        {
            *curr = *first;
            ++curr;
            ++prev;
            ++first;
        }

        if (first != last)
        {
            this->InsertAfter(prev, first, last);
        }
        else if (curr != end)
        {
            this->EraseAfter(prev, end);
        }
    }

  public:
    // These sortable function are only used for DSA
    template <typename TCompare> void InsertionSort(TCompare compare)
    {
        // this is an insertion sort implementation
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        NodeBaseAlias *curr = &this->LinkedListCore.NodeHead;
        while (auto *temp = static_cast<NodeAlias *>(curr->PointerNext))
        {
            NodeBaseAlias *next = curr->PointerNext;
            while (auto *nextTemp = static_cast<NodeAlias *>(next->PointerNext))
            {
                if (compare(*nextTemp->GetData(), *temp->GetData()))
                {
                    curr->TransferAfter(next, nextTemp);
                }
                else
                {
                    next = next->PointerNext;
                }
            }
            curr = curr->PointerNext;
        }
    }

    template <typename TCompare> void SelectionSort(TCompare compare)
    {
        // This is a selection sort implementation

        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        NodeBaseAlias *curr = &this->LinkedListCore.NodeHead;

        while (NodeAlias *temp = static_cast<NodeAlias *>(curr->PointerNext))
        {
            NodeBaseAlias *min = curr;
            NodeBaseAlias *next = curr->PointerNext;

            while (NodeAlias *nextTemp = static_cast<NodeAlias *>(next->PointerNext))
            {
                if (compare(*nextTemp->GetData(), *static_cast<NodeAlias *>(min->PointerNext)->GetData()))
                {
                    min = next;
                }
                next = next->PointerNext;
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

    template <typename TCompare> void BubbleSort(TCompare compare)
    {
        // This is a bubble sort implementation

        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        NodeBaseAlias *curr = &this->LinkedListCore.NodeHead;
        while (auto *temp = static_cast<NodeAlias *>(curr->PointerNext))
        {
            NodeBaseAlias *next = curr->PointerNext;
            while (auto *nextTemp = static_cast<NodeAlias *>(next->PointerNext))
            {
                if (compare(*nextTemp->GetData(), *static_cast<NodeAlias *>(next)->GetData()))
                {
                    curr->TransferAfter(next, nextTemp);
                }
                else
                {
                    next = next->PointerNext;
                }
            }
            curr = curr->PointerNext;
        }
    }

  private:
    template <typename TCompare> IteratorAlias QuickSortHelper(TCompare compare, IteratorAlias begin, IteratorAlias end)
    {
        if (begin == end || begin.PointerNext() == end)
        {
            return begin;
        }

        IteratorAlias pivot = PartitionForQuickSortHelper(compare, begin, end);

        QuickSortHelper(compare, begin, pivot);
        QuickSortHelper(compare, pivot.PointerNext(), end);

        return pivot;
    }

    template <typename TCompare>
    IteratorAlias PartitionForQuickSortHelper(TCompare compare, IteratorAlias begin, IteratorAlias end)
    {
        IteratorAlias pivot = begin;
        IteratorAlias i = begin;
        IteratorAlias j = begin.PointerNext();

        while (j != end)
        {
            if (compare(*j, *pivot))
            {
                ++i;
                std::swap(*i, *j);
            }
            ++j;
        }

        std::swap(*i, *pivot);

        return i;
    }

    template <typename TCompare> void HeapifyHelper(SizeTypeAlias root, SizeTypeAlias length, TCompare compare)
    {
        SizeTypeAlias largest = root;
        SizeTypeAlias left = 2 * root + 1;
        SizeTypeAlias right = 2 * root + 2;

        if (left < length && !compare(*FindForHeapSortHelper(left), *FindForHeapSortHelper(largest)))
        {
            largest = left;
        }

        if (right < length && !compare(*FindForHeapSortHelper(right), *FindForHeapSortHelper(largest)))
        {
            largest = right;
        }

        if (largest != root)
        {
            SwapForHeapSort(root, largest);
            HeapifyHelper(largest, length, compare);
        }
    }

    void SwapForHeapSort(SizeTypeAlias root, SizeTypeAlias left)
    {
        IteratorAlias rootIter = this->GetBegin();
        IteratorAlias leftIter = this->GetBegin();

        for (SizeTypeAlias i = 0; i < root && rootIter.PointerNext() != this->GetEnd(); i++)
        {
            ++rootIter;
        }

        for (SizeTypeAlias i = 0; i < left && leftIter.PointerNext() != this->GetEnd(); i++)
        {
            ++leftIter;
        }

        std::swap(*rootIter, *leftIter);
    }

    IteratorAlias FindForHeapSortHelper(SizeTypeAlias index)
    {
        IteratorAlias iter = this->GetBegin();
        for (SizeTypeAlias i = 0; i < index && iter != this->GetEnd(); i++)
        {
            ++iter;
        }
        return iter;
    }

  public:
    template <typename TCompare> void QuickSort(TCompare compare)
    {
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        QuickSortHelper(compare, this->GetBegin(), this->GetEnd());
    }

    template <typename TCompare> void HeapSort(TCompare compare)
    {
        SizeTypeAlias length = this->GetSize();

        if (length <= 1)
        {
            return;
        }

        for (SizeTypeAlias i = length / 2 - 1; i >= 0; i--)
        {
            HeapifyHelper(i, length, compare);
            if (i == 0)
            {
                break;
            }
        }

        for (SizeTypeAlias i = length - 1; i > 0; --i)
        {
            SwapForHeapSort(0, i);
            HeapifyHelper(0, i, compare);
        }
    }
};

template <typename T, typename Alloc> bool operator==(const SList<T, Alloc> &lhs, const SList<T, Alloc> &rhs)
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

template <typename T, typename Alloc> inline bool operator!=(const SList<T, Alloc> &lhs, const SList<T, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc> inline void Swap(SList<T, Alloc> &lhs, SList<T, Alloc> &rhs) noexcept
{
    lhs.Swap(rhs);
}
}; // namespace DSALibraries::Containers
#endif
