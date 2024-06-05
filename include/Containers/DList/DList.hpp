#ifndef DSA_LIBRARIES_DLIST_HPP
#define DSA_LIBRARIES_DLIST_HPP
#include "../../Utilities/Allocator.hpp"
#include "DListIterator.hpp"
#include "DListNode.hpp"
#include <memory>
#include <type_traits>
#include <utility>

using namespace DSALibraries::Utilities;

namespace DSALibraries::Containers
{
template <typename T, typename Alloc> class DListBase
{
  protected:
    using TypeAllocatorTraitsAlias = std::allocator_traits<Alloc>;
    using NodeAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<DListNode<T>>;
    using TpAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<T>;

    struct DListBaseCore : NodeAllocatorTypeAlias
    {
        DListNodeBase NodeHead;

        DListBaseCore() : NodeAllocatorTypeAlias(), NodeHead()
        {
        }

        explicit DListBaseCore(DListBaseCore&& list, NodeAllocatorTypeAlias&& nodeAllocator)
            : NodeAllocatorTypeAlias(std::move(nodeAllocator)), NodeHead(std::move(list.NodeHead))
        {
        }

        explicit DListBaseCore(NodeAllocatorTypeAlias&& nodeAllocator) noexcept
            : NodeAllocatorTypeAlias(std::move(nodeAllocator)), NodeHead()
        {
        }

        DListBaseCore(DListBaseCore&& list) = default;
    };

    DListBaseCore LinkedListCore;

    DListNode<T>* GetNodeInternal() noexcept
    {
        return static_cast<DListNode<T>*>(LinkedListCore.NodeAllocatorTypeAlias::allocate(1));
    }

    void PutNodeInternal(DListNode<T>* node) noexcept
    {
        LinkedListCore.NodeAllocatorTypeAlias::deallocate(node, 1);
    }

  public:
    using AllocatorTypeAlias = Alloc;
    using NodeAlias = DListNode<T>;
    using NodeBaseAlias = DListNodeBase;
    using IteratorAlias = DListIterator<T>;
    using ConstIteratorAlias = DListConstIterator<T>;

    NodeAllocatorTypeAlias& GetNodeAllocatorInternal() noexcept
    {
        return *static_cast<NodeAllocatorTypeAlias*>(&LinkedListCore);
    }

    const NodeAllocatorTypeAlias& GetNodeAllocatorInternal() const noexcept
    {
        return *static_cast<const NodeAllocatorTypeAlias*>(&LinkedListCore);
    }

    TpAllocatorTypeAlias GetTpAllocatorInternal() const noexcept
    {
        return TpAllocatorTypeAlias(GetNodeAllocatorInternal());
    }

    AllocatorTypeAlias GetAllocator() const noexcept
    {
        return AllocatorTypeAlias(GetNodeAllocatorInternal());
    }

    DListBase() = default;

    explicit DListBase(NodeAllocatorTypeAlias&& allocator) : LinkedListCore(std::move(allocator))
    {
    }

    DListBase(DListBase&& list, NodeAllocatorTypeAlias&& al, std::true_type) noexcept
        : LinkedListCore(std::move(list.LinkedListCore), std::move(al))
    {
    }

    DListBase(DListBase&& list, NodeAllocatorTypeAlias&& al) noexcept
        : LinkedListCore(std::move(list.LinkedListCore), std::move(al))
    {
        if (list.GetNodeAllocatorInternal() == al)
        {
            this->LinkedListCore.NodeHead = std::move(list.LinkedListCore.NodeHead);
        }
    }

    ~DListBase() noexcept
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead, nullptr);
    }

  protected:
    template <typename... Args> NodeAlias* CreateNodeInternal(Args&&... args)
    {
        NodeAlias* node = this->GetNodeInternal();
        try
        {
            this->GetTpAllocatorInternal().construct(node->GetData(), std::forward<Args>(args)...);
        }
        catch (...)
        {
            this->PutNodeInternal(node);
            throw;
        }
        return node;
    }
    template <typename... Args> IteratorAlias InsertBeforeInternal(ConstIteratorAlias position, Args&&... args)
    {
        NodeBaseAlias* nodeBase = const_cast<NodeBaseAlias*>(position.NodeBase);

        NodeAlias* newNode = this->CreateNodeInternal(std::forward<Args>(args)...);

        newNode->HookBeforeInternal(nodeBase);

        return IteratorAlias(newNode);
    }

    template <typename... Args> IteratorAlias InsertAfterInternal(ConstIteratorAlias position, Args&&... args)
    {
        NodeBaseAlias* nodeBase = const_cast<NodeBaseAlias*>(position.NodeBase);

        NodeAlias* newNode = this->CreateNodeInternal(std::forward<Args>(args)...);

        newNode->HookAfterInternal(nodeBase);

        return IteratorAlias(newNode);
    }

    NodeBaseAlias* EraseAfterInternal(NodeBaseAlias* position)
    {
        NodeAlias* currentNode = static_cast<NodeAlias*>(position->PointerNext);

        currentNode->UnhookInternal();

        this->GetTpAllocatorInternal().destroy(currentNode->GetData());

        currentNode->~NodeAlias();

        this->PutNodeInternal(static_cast<NodeAlias*>(currentNode));

        return position->PointerNext;
    }

    NodeBaseAlias* EraseAfterInternal(NodeBaseAlias* begin, NodeBaseAlias* end)
    {
        NodeAlias* currentNode = static_cast<NodeAlias*>(begin->PointerNext);
        while (currentNode != end)
        {
            NodeAlias* temp = currentNode;
            currentNode = static_cast<NodeAlias*>(currentNode->PointerNext);
            this->GetTpAllocatorInternal().destroy(temp->GetData());
            temp->~NodeAlias();
            this->PutNodeInternal(temp);
        }

        begin->PointerNext = end;

        if (end)
        {
            end->PointerPrev = begin;
        }

        return end;
    }
};

template <typename T, typename Alloc = Allocator<T>> class DList : protected DListBase<T, Alloc>
{

  private:
    using DListBaseAlias = DListBase<T, Alloc>;
    using NodeAllocatorTypeAlias = typename DListBaseAlias::NodeAllocatorTypeAlias;
    using TpAllocatorTypeAlias = typename DListBaseAlias::TpAllocatorTypeAlias;

  public:
    using ValueTypeAlias = T;
    using ReferenceTypeAlias = ValueTypeAlias&;
    using ConstReferenceTypeAlias = const ValueTypeAlias&;
    using IteratorAlias = typename DListBaseAlias::IteratorAlias;
    using ConstIteratorAlias = typename DListBaseAlias::ConstIteratorAlias;
    using AllocatorTypeAlias = typename DListBaseAlias::AllocatorTypeAlias;
    using NodeAlias = typename DListBaseAlias::NodeAlias;
    using NodeBaseAlias = typename DListBaseAlias::NodeBaseAlias;
    using SizeTypeAlias = size_t;

    // Adaptive alias
    using value_type = ValueTypeAlias;
    using reference = ReferenceTypeAlias;
    using const_reference = ConstReferenceTypeAlias;
    using iterator = IteratorAlias;
    using const_iterator = ConstIteratorAlias;
    using allocator_type = AllocatorTypeAlias;
    using size_type = SizeTypeAlias;

  private:
    DList(DList&& list, NodeAllocatorTypeAlias&& nodeAllocator, std::false_type) noexcept
        : DListBaseAlias(std::move(list), std::move(nodeAllocator))
    {
        this->InsertAfter(this->GetConstBeginFromHead(), list.GetBegin(), list.GetEnd());
    }

    DList(DList&& list, NodeAllocatorTypeAlias&& nodeAllocator, std::true_type) noexcept
        : DListBaseAlias(std::move(list), std::move(nodeAllocator))
    {
    }

  public:
    DList() : DListBaseAlias()
    {
    }

    explicit DList(const AllocatorTypeAlias& allocator) : DListBaseAlias(AllocatorTypeAlias(allocator))
    {
    }

    explicit DList(SizeTypeAlias size, const AllocatorTypeAlias& al = Alloc())
        : DListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        DefaultInitializeInternal(size);
    }

    DList(SizeTypeAlias size, const ValueTypeAlias& value, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : DListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        FillInitializeInternal(size, value);
    }

    DList(const DList& list) : DListBaseAlias(NodeAllocatorTypeAlias::select_on_container_copy_construction())
    {
        RangeInitalizeInternal(list.GetBegin(), list.GetEnd());
    }

    DList(std::initializer_list<ValueTypeAlias> init, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : DListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitalizeInternal(init.begin(), init.end());
    }

    template <typename InputIterator>
    DList(InputIterator begin, InputIterator end, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : DListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitalizeInternal(begin, end);
    }

    DList(DList&& list, const AllocatorTypeAlias& al) noexcept
        : DList(std::move(list), NodeAllocatorTypeAlias(al), NodeAllocatorTypeAlias::is_always_equal())
    {
    }

    ~DList() noexcept = default;

    DList& operator=(const DList& list)
    {
        if (this != std::addressof(list))
        {
            if (this->GetAllocator() == list.GetAllocator())
            {
                this->AssignInternal(list.GetConstBegin(), list.GetConstEnd(),
                                     std::is_copy_assignable<ValueTypeAlias>());
            }
            else
            {
                this->AssignInternal(list.GetConstBegin(), list.GetConstEnd(), std::false_type());
            }
        }
        return *this;
    }

    DList& operator=(DList&& list) noexcept
    {
        this->Clear();
        this->GetNodeAllocatorInternal() = std::move(list.GetNodeAllocatorInternal());
        this->MoveAssignInternal(std::move(list), std::is_nothrow_move_assignable<AllocatorTypeAlias>());
    }

    //    AllocatorTypeAlias GetAllocator() const noexcept
    //    {
    //        return DListBaseAlias::GetAllocator();
    //    }

    // Start Zone Adaptive Iterator
    IteratorAlias begin() noexcept
    {
        return this->GetBegin();
    }

    ConstIteratorAlias begin() const noexcept
    {
        return this->GetConstBegin();
    }

    [[maybe_unused]] ConstIteratorAlias cbegin() const noexcept
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

    [[maybe_unused]] IteratorAlias before_begin() noexcept
    {
        return this->GetBeginFromHead();
    }

    [[maybe_unused]] ConstIteratorAlias before_begin() const noexcept
    {
        return this->GetConstBeginFromHead();
    }

    [[maybe_unused]] ConstIteratorAlias cbefore_begin() const noexcept
    {
        return this->GetConstBeginFromHead();
    }
    // End Zone Adaptive Iterator

    ConstIteratorAlias GetConstBeginFromHead() const noexcept
    {
        return ConstIteratorAlias(&this->LinkedListCore.NodeHead);
    }

    IteratorAlias GetBeginFromHead() noexcept
    {
        return IteratorAlias(&this->LinkedListCore.NodeHead);
    }

    ConstIteratorAlias GetConstBegin() const noexcept
    {
        return ConstIteratorAlias(this->LinkedListCore.NodeHead.PointerNext);
    }

    IteratorAlias GetBegin() noexcept
    {
        return IteratorAlias(this->LinkedListCore.NodeHead.PointerNext);
    }

    ConstIteratorAlias GetConstEnd() const noexcept
    {
        return ConstIteratorAlias(nullptr);
    }

    IteratorAlias GetEnd() noexcept
    {
        return IteratorAlias(nullptr);
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return this->LinkedListCore.NodeHead.PointerNext == nullptr;
    }

    [[nodiscard]] SizeTypeAlias GetMaxSize() const noexcept
    {
        return this->GetNodeAllocatorInternal().max_size();
    }

    [[nodiscard]] SizeTypeAlias GetSize() const noexcept
    {
        SizeTypeAlias size = 0;
        for (ConstIteratorAlias it = GetConstBegin(); it != GetConstEnd(); ++it)
        {
            ++size;
        }
        return size;
    }

    void Resize(SizeTypeAlias newSize)
    {
    }

    ReferenceTypeAlias GetItemFront() noexcept
    {
        return *GetBegin();
    }

    ConstReferenceTypeAlias GetConstItemFront() const noexcept
    {
        return *GetConstBegin();
    }

    void PushFront(const ValueTypeAlias& value)
    {
        this->InsertAfterInternal(this->GetBeginFromHead(), value);
    }

    void PushFront(ValueTypeAlias&& value)
    {
        this->InsertAfterInternal(this->GetBeginFromHead(), std::move(value));
    }

    template <typename... Args> void EmplaceFront(Args&&... args)
    {
        this->InsertAfterInternal(this->GetBeginFromHead(), std::forward<Args>(args)...);
    }

    void PopFront()
    {
        this->EraseAfterInternal(GetBeginFromHead());
    }

    template <typename... Args> IteratorAlias EmplaceBefore(ConstIteratorAlias position, Args&&... args)
    {
        return IteratorAlias(this->InsertBeforeInternal(position, std::forward<Args>(args)...));
    }

    template <typename... Args> IteratorAlias EmplaceAfter(ConstIteratorAlias position, Args&&... args)
    {
        return IteratorAlias(this->InsertAfterInternal(position, std::forward<Args>(args)...));
    }

    IteratorAlias InsertBefore(ConstIteratorAlias position, const ValueTypeAlias& value)
    {
        return IteratorAlias(this->InsertBeforeInternal(position, value));
    }

    IteratorAlias InsertBefore(ConstIteratorAlias position, ValueTypeAlias&& value)
    {
        return IteratorAlias(this->InsertBeforeInternal(position, std::move(value)));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias position, const ValueTypeAlias& value)
    {
        return this->InsertAfterInternal(position, value);
    }

    IteratorAlias InsertAfter(ConstIteratorAlias position, ValueTypeAlias&& value)
    {
        return this->InsertAfterInternal(position, std::move(value));
    }

    IteratorAlias InsertAfter(ConstIteratorAlias position, SizeTypeAlias size, const ValueTypeAlias& value)
    {
        if (size)
        {
            DList temp(size, value, this->GetAllocator());
            return this->SpliceAfterInternal(position, temp.GetBeginFromHead(), temp.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<IteratorAlias*>(position.NodeBase));
        }
    }

    template <typename InputIterator>
    void InsertAfter(ConstIteratorAlias position, InputIterator begin, InputIterator end)
    {
        DList list(begin, end, this->GetAllocator());

        if (!list.IsEmpty())
        {
            return this->SpliceAfterInternal(position, list.GetConstBeginFromHead(), list.GetConstEnd());
        }
        else
        {
            return IteratorAlias(const_cast<IteratorAlias*>(position.NodeBase));
        }
    }

    void Assign(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        MoveAssignInternal(size, value, std::is_copy_assignable<ValueTypeAlias>());
    }

    template <typename InputIterator> void Assign(InputIterator begin, InputIterator end)
    {
        MoveAssignInternal(begin, end, std::is_copy_assignable<ValueTypeAlias>());
    }

    void SpliceAfter(ConstIteratorAlias pos, DList&& list)
    {
        if (!list.IsEmpty())
        {
            this->SpliceAfterInternal(pos, list.GetBeginFromHead(), list.GetConstEnd());
        }
    }

    void SpliceAfter(ConstIteratorAlias pos, DList& list)
    {
        SpliceAfter(pos, std::move(list));
    }

    void SpliceAfter(ConstIteratorAlias pos, DList&& list, ConstIteratorAlias it)
    {
        ConstIteratorAlias next = it;
        ++next;
        if (pos == it || pos == next)
        {
            return;
        }
        NodeBaseAlias* begin = const_cast<NodeBaseAlias*>(pos.NodeBase);
        return begin->TransferAfter(const_cast<NodeBaseAlias*>(it.NodeBase), const_cast<NodeBaseAlias*>(next.NodeBase));
    }

    void SpliceAfter(ConstIteratorAlias pos, DListBaseAlias&& list, ConstIteratorAlias begin, ConstIteratorAlias end)
    {
        this->SpliceAfterInternal(pos, begin, end);
    }

    void SpliceAfter(ConstIteratorAlias pos, DList& list, ConstIteratorAlias first, ConstIteratorAlias last)
    {
        SpliceAfter(pos, std::move(list), first, last);
    }

    IteratorAlias EraseAfter(ConstIteratorAlias position)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias*>(position.NodeBase)));
    }

    IteratorAlias EraseAfter(ConstIteratorAlias begin, ConstIteratorAlias end)
    {
        return IteratorAlias(this->EraseAfterInternal(const_cast<NodeBaseAlias*>(begin.NodeBase),
                                                      const_cast<NodeBaseAlias*>(end.NodeBase)));
    }

    void Remove(const ValueTypeAlias& value)
    {
        NodeBaseAlias* node = static_cast<NodeBaseAlias*>(&this->LinkedListCore.NodeHead);
        NodeBaseAlias* saveNode = nullptr;

        while (NodeAlias* temp = static_cast<NodeAlias*>(node->PointerNext))
        {
            if (*temp->GetData() == value)
            {
                if (temp->GetData() != std::addressof(value))
                {
                    this->EraseAfterInternal(node);
                    continue;
                }
                else
                {
                    saveNode = node;
                }
            }

            node = node->PointerNext;
        }

        if (saveNode)
        {
            this->EraseAfterInternal(saveNode);
        }
    }

    template <typename TPredicate> void RemoveIf(TPredicate predicate)
    {
        NodeBaseAlias* node = &this->LinkedListCore.NodeHead;
        while (NodeAlias* temp = static_cast<NodeAlias*>(node->PointerNext))
        {
            if (predicate(*temp->GetData()))
            {
                this->EraseAfterInternal(node);
            }
            else
            {
                node = node->PointerNext;
            }
        }
    }

    void Merge(DList&& list)
    {
        Merge(std::move(list), std::less<ValueTypeAlias>());
    }

    void Merge(DList& list)
    {
        Merge(std::move(list));
    }

    template <typename TCompare> void Merge(DList&& list, TCompare compare)
    {
        if (std::addressof(list) == this)
        {
            return;
        }

        NodeBaseAlias* node = static_cast<NodeBaseAlias*>(&this->LinkedListCore.NodeHead);

        while (node->PointerNext && list.LinkedListCore.NodeHead.PointerNext)
        {
            if (compare(*static_cast<NodeAlias*>(list.LinkedListCore.NodeHead.PointerNext)->GetData(),
                        *static_cast<NodeAlias*>(node->PointerNext)->GetData()))
            {
                node->TransferAfter(&list.LinkedListCore.NodeHead, list.LinkedListCore.NodeHead.PointerNext);
            }

            node = node->PointerNext;
        }

        if (list.LinkedListCore.NodeHead.PointerNext)
        {
            *node = std::move(list.LinkedListCore.NodeHead);
        }
    }

    template <typename Compare> void Merge(DList& list, Compare compare)
    {
        Merge(std::move(list), compare);
    }

    void Sort()
    {
        this->Sort(std::less<ValueTypeAlias>());
    }

    template <typename TCompare> void Sort(TCompare compare)
    {
        // MergeHelper Sort for doubly linked list

        IteratorAlias pLeft;
        IteratorAlias pRight;
        IteratorAlias pNode;
        IteratorAlias pHead;
        IteratorAlias pTail;

        SizeTypeAlias listSize = 1;
        SizeTypeAlias numOfMerges;
        SizeTypeAlias leftSize;
        SizeTypeAlias rightSize;

        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        while (true)
        {
            pLeft = this->GetBegin();
            pHead = this->GetBeginFromHead();
            pTail = this->GetBeginFromHead();

            numOfMerges = 0;

            while (pLeft != this->GetEnd())
            {
                ++numOfMerges;
                pRight = pLeft;
                leftSize = 0;

                for (SizeTypeAlias i = 0; i < listSize; ++i)
                {
                    ++pRight;
                    ++leftSize;
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

                    if (pTail == this->GetBeginFromHead())
                    {
                        pHead.NodeBase->PointerNext = pNode.NodeBase;
                        pNode.NodeBase->PointerPrev = pHead.NodeBase;
                        pHead = pNode;
                        pTail = pNode;
                    }
                    else
                    {
                        pTail.NodeBase->PointerNext = pNode.NodeBase;
                        pNode.NodeBase->PointerPrev = pTail.NodeBase;
                        pTail = pNode;
                    }
                    pTail.NodeBase->PointerNext = nullptr;
                }
                pLeft = pRight;
            }

            if (numOfMerges <= 1)
            {
                return;
            }
            listSize *= 2;
        }
    }

    void Reverse() noexcept
    {
        this->LinkedListCore.NodeHead.ReverseAfter();
    }

    void Swap(DList& list) noexcept
    {
        std::swap(this->LinkedListCore.NodeHead.PointerNext, list.LinkedListCore.NodeHead.PointerNext);
        std::swap(this->LinkedListCore.NodeHead.PointerNext.PointerPrev,
                  list.LinkedListCore.NodeHead.PointerNext.PointerPrev);
    }

    void Clear() noexcept
    {
        this->EraseAfterInternal(&this->LinkedListCore.NodeHead, nullptr);
    }

  private:
    void DefaultInitializeInternal(SizeTypeAlias size)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;

        for (; size > 0; --size)
        {
            NodeAlias* newNode = this->CreateNodeInternal();
            newNode->HookAfterInternal(temp);
            temp = newNode;
        }
    }

    void FillInitializeInternal(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;

        for (; size > 0; --size)
        {
            NodeAlias* newNode = this->CreateNodeInternal(value);
            newNode->HookAfterInternal(temp);
            temp = newNode;
        }
    }

    template <typename InputIterator> void RangeInitalizeInternal(InputIterator begin, InputIterator end)
    {
        NodeBaseAlias* temp = &this->LinkedListCore.NodeHead;

        for (; begin != end; ++begin)
        {
            NodeAlias* newNode = this->CreateNodeInternal(*begin);
            newNode->HookAfterInternal(temp);
            temp = newNode;
        }
    }

    template <typename InputIterator> void AssignInternal(InputIterator begin, InputIterator end, std::true_type)
    {
        IteratorAlias* prev = GetBeginFromHead();
        IteratorAlias* current = GetBegin();
        IteratorAlias* last = GetEnd();

        while (begin != end && current != last)
        {
            *current = *begin;
            ++begin;
            ++current;
            ++prev;
        }

        if (begin != end)
        {
            this->InsertAfterInternal(prev, begin, end);
        }
        else
        {
            this->EraseAfterInternal(prev, last);
        }
    }

    template <typename InputIterator> void AssignInternal(InputIterator begin, InputIterator end, std::false_type)
    {
        this->Clear();
        this->InsertAfter(this->GetConstBeginFromHead(), begin, end);
    }

    void MoveAssignInternal(DList&& list, std::true_type)
    {
        this->Clear();
        this->InsertAfter(this->GetConstBeginFromHead(), list.GetBegin(), list.GetEnd());
    }

    void MoveAssignInternal(DList&& list, std::false_type)
    {
        if (this->GetAllocator() == list.GetAllocator())
        {
            MoveAssignInternal(std::move(list), std::true_type());
        }
        else
        {
            this->Clear();
            this->AssignInternal(list.GetConstBegin(), list.GetConstEnd(), std::false_type());
        }
    }

    IteratorAlias SpliceAfterInternal(ConstIteratorAlias pos, ConstIteratorAlias begin, ConstIteratorAlias end)
    {
        NodeBaseAlias* temp = const_cast<NodeBaseAlias*>(pos.NodeBase);
        NodeBaseAlias* beginNode = const_cast<NodeBaseAlias*>(begin.NodeBase);
        NodeBaseAlias* endNode = beginNode;

        while (endNode->PointerNext != end.NodeBase)
        {
            endNode = endNode->PointerNext;
        }

        if (beginNode != endNode)
        {
            temp->TransferAfter(beginNode, endNode);
        }
        else
        {
            return IteratorAlias(temp);
        }
    }

    // Zone of DSA

  public:
    template <typename TCompare> void BubbleSort(TCompare compare)
    {
        // Bubble Sort for doubly linked list, inplace sorting
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }
        while (true)
        {
            bool isSwapped = false;
            NodeBaseAlias* curr = &this->LinkedListCore.NodeHead;
            while (NodeAlias* temp = static_cast<NodeAlias*>(curr->PointerNext))
            {
                if (NodeAlias* nextTemp = static_cast<NodeAlias*>(temp->PointerNext))
                {
                    if (compare(*nextTemp->GetData(), *temp->GetData()))
                    {
                        curr->TransferAfter(temp, nextTemp);
                        isSwapped = true;
                    }
                }
                curr = curr->PointerNext;
            }
            if (!isSwapped)
            {
                break;
            }
        }
    }

    template <typename TCompare> void SelectionSort(TCompare compare)
    {
        // Selection Sort for doubly linked list, inplace sorting
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        NodeBaseAlias* curr = &this->LinkedListCore.NodeHead;

        while (NodeAlias* temp = static_cast<NodeAlias*>(curr->PointerNext))
        {
            NodeBaseAlias* min = curr;
            NodeBaseAlias* next = curr->PointerNext;

            while (NodeAlias* nextTemp = static_cast<NodeAlias*>(next->PointerNext))
            {
                if (compare(*nextTemp->GetData(), *static_cast<NodeAlias*>(min->PointerNext)->GetData()))
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

    template <typename TCompare> void InsertionSort(TCompare compare)
    {
        // Insertion Sort for doubly linked list, inplace sorting
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        NodeBaseAlias* sorted = this->LinkedListCore.NodeHead.PointerNext;
        NodeBaseAlias* unsorted = sorted->PointerNext;

        while (sorted->PointerNext)
        {
            bool isInserted = false;
            for (NodeBaseAlias* temp = &this->LinkedListCore.NodeHead; temp->PointerNext != sorted;
                 temp = temp->PointerNext)
            {
                if (compare(*static_cast<NodeAlias*>(temp->PointerNext)->GetData(),
                            *static_cast<NodeAlias*>(unsorted)->GetData()))
                {
                    temp->TransferAfter(sorted, unsorted);
                    unsorted = sorted->PointerNext;
                    isInserted = true;
                    break;
                }
            }

            if (!isInserted)
            {
                sorted = sorted->PointerNext;
                unsorted = sorted->PointerNext;
            }
        }
    }

    template <typename TCompare> void HeapSort(TCompare compare)
    {
        // Heap Sort for doubly linked list
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        SizeTypeAlias length = GetSize();
        for (SizeTypeAlias i = length / 2 - 1; i >= 0; --i)
        {
            HeapifyHelper(i, length, compare);
            if (i == 0)
            {
                break;
            }
        }

        for (SizeTypeAlias i = length - 1; i > 0; --i)
        {
            SwapForHeapSortHelper(0, i);
            HeapifyHelper(0, i, compare);
        }
    }

    template <typename TCompare> void QuickSort(TCompare compare)
    {
        // Quick Sort for doubly linked list
        if (this->IsEmpty() || this->GetBegin().PointerNext() == this->GetEnd())
        {
            return;
        }

        QuickSortHelper(compare, this->GetBegin(), this->GetEnd());
    }

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

  private:
    template <typename TCompare> IteratorAlias QuickSortHelper(TCompare compare, IteratorAlias begin, IteratorAlias end)
    {
        if (begin == end || begin.PointerNext() == end)
        {
            return begin;
        }

        IteratorAlias pivot = this->PartitionForQuickSortHelper(compare, begin, end);

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
            SwapForHeapSortHelper(root, largest);
            HeapifyHelper(largest, length, compare);
        }
    }

    void SwapForHeapSortHelper(SizeTypeAlias root, SizeTypeAlias left)
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
};

template <typename T, typename Alloc>
inline bool operator==(const DList<T, Alloc>& lhs, const DList<T, Alloc>& rhs) noexcept
{
    typedef typename DList<T, Alloc>::ConstIteratorAlias ConstIteratorAlias;

    ConstIteratorAlias it1 = lhs.GetConstBegin();
    ConstIteratorAlias it2 = rhs.GetConstBegin();

    ConstIteratorAlias end1 = lhs.GetConstEnd();
    ConstIteratorAlias end2 = rhs.GetConstEnd();

    while (it1 != end1 && it2 != end2 && *it1 == *it2)
    {
        ++it1;
        ++it2;
    }

    return it1 == end1 && it2 == end2;
}

template <typename T, typename Alloc>
inline bool operator!=(const DList<T, Alloc>& lhs, const DList<T, Alloc>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc> inline void Swap(DList<T, Alloc>& lhs, DList<T, Alloc>& rhs) noexcept
{
    lhs.Swap(rhs);
}
} // namespace DSALibraries::Containers
#endif // DSA_LIBRARIES_DLIST_HPP
