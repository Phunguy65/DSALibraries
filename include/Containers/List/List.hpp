
#ifndef DSA_LIBRARIES_LIST_HPP
#define DSA_LIBRARIES_LIST_HPP 1
#include "../../Utilities/Allocator.hpp"
#include "ListIterator.hpp"
#include "ListNode.hpp"
using namespace DSALibraries::Utilities;

namespace DSALibraries::Containers
{
template <typename T, typename Alloc> class ListBase
{
  protected:
    using TypeAllocatorTraitsAlias = std::allocator_traits<Alloc>;
    using NodeAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<ListNode<T>>;
    using TpAllocatorTypeAlias = typename TypeAllocatorTraitsAlias::template rebind_alloc<T>;

    struct ListBaseCore : NodeAllocatorTypeAlias
    {
        ListNodeBase NodeBase;

        ListBaseCore() : NodeAllocatorTypeAlias(), NodeBase()
        {
        }

        explicit ListBaseCore(const NodeAllocatorTypeAlias& nodeAllocator)
            : NodeAllocatorTypeAlias(nodeAllocator), NodeBase()
        {
        }

        explicit ListBaseCore(NodeAllocatorTypeAlias&& nodeAllocator) noexcept
            : NodeAllocatorTypeAlias(std::move(nodeAllocator)), NodeBase()
        {
        }
    };

    ListBaseCore LinkedListCore;

    ListNode<T>* GetNodeInternal() noexcept
    {
        return static_cast<ListNode<T>*>(LinkedListCore.NodeAllocatorTypeAlias::allocate(1));
    }

    void PutNodeInternal(ListNode<T>* node) noexcept
    {
        LinkedListCore.NodeAllocatorTypeAlias::deallocate(node, 1);
    }

  public:
    using AllocatorTypeAlias = Alloc;
    using NodeAlias = ListNode<T>;
    using IteratorAlias = ListIterator<T>;
    using ConstIteratorAlias = ListConstIterator<T>;

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

    ListBase() : LinkedListCore()
    {
        InitInternal();
    }

    explicit ListBase(const AllocatorTypeAlias& allocator) : LinkedListCore(allocator)
    {
        InitInternal();
    }

    ListBase(ListBase&& list) noexcept : LinkedListCore(std::move(list.LinkedListCore))
    {
        InitInternal();
        ::DSALibraries::Containers::ListNodeBase::Swap(LinkedListCore.NodeBase, list.LinkedListCore.NodeBase);
    }

    ~ListBase() noexcept
    {
        ClearInternal();
    }

  protected:
    void InitInternal() noexcept
    {
        LinkedListCore.NodeBase.PointerNext = &LinkedListCore.NodeBase;
        LinkedListCore.NodeBase.PointerPrev = &LinkedListCore.NodeBase;
    }

    void ClearInternal() noexcept
    {
        auto* currentNode = static_cast<NodeAlias*>(this->LinkedListCore.NodeBase.PointerNext);
        while (currentNode != &this->LinkedListCore.NodeBase)
        {
            NodeAlias* temp = currentNode;
            currentNode = static_cast<NodeAlias*>(currentNode->PointerNext);
            this->GetTpAllocatorInternal().destroy(temp->GetData());
            temp->~NodeAlias();
            PutNodeInternal(temp);
        }
    }
};

template <typename T, typename Alloc = Allocator<T>> class List : protected ListBase<T, Alloc>
{

  private:
    using ListBaseAlias = ListBase<T, Alloc>;
    using NodeAllocatorTypeAlias = typename ListBaseAlias::NodeAllocatorTypeAlias;
    using TpAllocatorTypeAlias = typename ListBaseAlias::TpAllocatorTypeAlias;

  public:
    using ValueTypeAlias = T;
    using ReferenceTypeAlias = ValueTypeAlias&;
    using ConstReferenceTypeAlias = const ValueTypeAlias&;

    using IteratorAlias = ListBaseAlias::IteratorAlias;
    using ConstIteratorAlias = ListBaseAlias::ConstIteratorAlias;
    using AllocatorTypeAlias = typename ListBaseAlias::AllocatorTypeAlias;
    using NodeAlias = typename ListBaseAlias::NodeAlias;

    using ReverseIteratorAlias = std::reverse_iterator<IteratorAlias>;
    using ConstReverseIteratorAlias = std::reverse_iterator<ConstIteratorAlias>;
    using SizeTypeAlias = size_t;

  protected:
    NodeAlias* CreateNodeInternal(const ValueTypeAlias& value)
    {
        NodeAlias* node = this->GetNodeInternal();
        try
        {
            this->GetTpAllocatorInternal().construct(node->GetData(), std::move(value));
        }
        catch (...)
        {
            this->PutNodeInternal(node);
            throw;
        }
        return node;
    }

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

  public:
    List() : ListBaseAlias()
    {
    }

    explicit List(const AllocatorTypeAlias& allocator) : ListBaseAlias(AllocatorTypeAlias(allocator))
    {
    }

    explicit List(SizeTypeAlias size) : ListBaseAlias()
    {
        DefaultInitializeInternal(size);
    }

    List(SizeTypeAlias size, const ValueTypeAlias& value, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : ListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        FillInitializeInternal(size, value);
    }

    List(const List& list) : ListBaseAlias(list.GetNodeAllocatorInternal())
    {
        RangeInitalizeInternal(list.Begin(), list.GetEnd());
    }

    List(List&& list) noexcept : ListBaseAlias(std::move(list))
    {
    }

    List(std::initializer_list<ValueTypeAlias> init, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : ListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitalizeInternal(init.begin(), init.end());
    }

    template <typename InputIterator>
    List(InputIterator begin, InputIterator end, const AllocatorTypeAlias& al = AllocatorTypeAlias())
        : ListBaseAlias(NodeAllocatorTypeAlias(al))
    {
        RangeInitalizeInternal(begin, end);
    }

    List& operator=(const List& list)
    {
        if (this != &list)
        {
            List temp(list);
            this->Swap(temp);
        }
        return *this;
    }

    List& operator=(List&& list) noexcept
    {
        this->Swap(list);
        this->Clear();
        return *this;
    }

    AllocatorTypeAlias GetAllocator() const noexcept
    {
        return ListBaseAlias::GetAllocator();
    }

    ConstIteratorAlias GetConstBegin() const noexcept
    {
        return ConstIteratorAlias(this->LinkedListCore.NodeBase.PointerNext);
    }

    IteratorAlias GetBegin() noexcept
    {
        return IteratorAlias(this->LinkedListCore.NodeBase.PointerNext);
    }

    ConstIteratorAlias GetConstEnd() const noexcept
    {
        return ConstIteratorAlias(&this->LinkedListCore.NodeBase);
    }

    IteratorAlias GetEnd() noexcept
    {
        return IteratorAlias(&this->LinkedListCore.NodeBase);
    }

    ConstReverseIteratorAlias GetReverseConstBegin() const noexcept
    {
        return ConstReverseIteratorAlias(GetEnd());
    }

    ReverseIteratorAlias GetReverseBegin() noexcept
    {
        return ReverseIteratorAlias(GetEnd());
    }

    ConstReverseIteratorAlias GetReverseConstEnd() const noexcept
    {
        return ConstReverseIteratorAlias(GetBegin());
    }

    ReverseIteratorAlias GetReverseEnd() noexcept
    {
        return ReverseIteratorAlias(GetBegin());
    }

    bool IsEmpty() const noexcept
    {
        return this->LinkedListCore.NodeBase.PointerNext == &this->LinkedListCore.NodeBase;
    }

    SizeTypeAlias GetMaxSize() const noexcept
    {
        return this->GetNodeAllocatorInternal().max_size();
    }

    SizeTypeAlias GetSize() const noexcept
    {
        SizeTypeAlias size = 0;
        for (ConstIteratorAlias it = GetConstBegin(); it != GetConstEnd(); ++it)
        {
            ++size;
        }
        return size;
    }

    void Resize(SizeTypeAlias newSize);

    ReferenceTypeAlias GetItemFront() noexcept
    {
        return *GetBegin();
    }

    ConstReferenceTypeAlias GetConstItemFront() const noexcept
    {
        return *GetConstBegin();
    }

    ReferenceTypeAlias GetItemBack() noexcept
    {
        return *(--GetEnd());
    }

    ConstReferenceTypeAlias GetConstItemBack() const noexcept
    {
        return *(--GetConstEnd());
    }

    // Đây là phương thức thay thế toàn bộ phần tử trong danh sách bằng một giá trị cụ thể
    void Assign(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        FillAssignInternal(size, value);
    }

    template <typename InputIterator> void Assign(InputIterator begin, InputIterator end)
    {
        RangeAssignInternal(begin, end);
    }

    void PushFront(const ValueTypeAlias& value)
    {
        InsertInternal(GetBegin(), value);
    }

    void PushFront(ValueTypeAlias&& value)
    {
        InsertInternal(GetBegin(), std::move(value));
    }

    template <typename... Args> void EmplaceFront(Args&&... args)
    {
        InsertInternal(GetBegin(), std::forward<Args>(args)...);
    }

    template <typename... Args> void EmplaceBack(Args&&... args)
    {
        InsertInternal(GetEnd(), std::forward<Args>(args)...);
    }

    void PopFront()
    {
        EraseInternal(GetBegin());
    }

    void PushBack(const ValueTypeAlias& value)
    {
        InsertInternal(GetEnd(), value);
    }

    void PushBack(ValueTypeAlias&& value)
    {
        InsertInternal(GetEnd(), std::move(value));
    }

    void PopBack()
    {
        EraseInternal(IteratorAlias(this->LinkedListCore.NodeBase.PointerPrev));
    }

    template <typename... Args> IteratorAlias EmplaceAfter(IteratorAlias& position, Args&&... args)
    {
        NodeAlias* newNode = this->CreateNodeInternal(std::forward<Args>(args)...);
        newNode->HookInternal(position.NodeBase);
        return IteratorAlias(newNode);
    }

    IteratorAlias Insert(const ConstIteratorAlias& position, const ValueTypeAlias& value)
    {
        return InsertInternal(position, value);
    }

    void Splice(IteratorAlias pos, List&& list)
    {
        if (!list.IsEmpty())
        {
            this->LinkedListCore.NodeBase.TransferAfterInternal(pos, list.GetBegin(), list.GetEnd());
        }
    }

    void Splice(IteratorAlias pos, List& list)
    {
        Splice(pos, std::move(list));
    }

    void Splice(IteratorAlias pos, List&& list, IteratorAlias it)
    {
        IteratorAlias next = it;
        ++next;
        if (pos == it || pos == next)
        {
            return;
        }
        this->TransferInternal(pos, it, next);
    }

    void Splice(IteratorAlias pos, List& list, IteratorAlias it)
    {
        Splice(pos, std::move(list), it);
    }

    void Splice(IteratorAlias pos, List&& list, IteratorAlias first, IteratorAlias last)
    {
        if (first != last)
        {
            this->TransferInternal(pos, first, last);
        }
    }

    void Splice(IteratorAlias pos, List& list, IteratorAlias first, IteratorAlias last)
    {
        Splice(pos, std::move(list), first, last);
    }

    IteratorAlias InsertAfter(IteratorAlias& position, ValueTypeAlias&& value)
    {
        return EmplaceAfter(position, std::move(value));
    }

    IteratorAlias Insert(IteratorAlias& position, const ValueTypeAlias& value)
    {
        NodeAlias* newNode = this->CreateNodeInternal(value);
        newNode->HookInternal(position.NodeBase);
        return IteratorAlias(newNode);
    }

    void Insert(IteratorAlias& position, SizeTypeAlias size, const ValueTypeAlias& value)
    {
        List temp(size, value, this->GetAllocator());
        Splice(position, std::move(temp));
    }

    template <typename InputIterator> void Insert(IteratorAlias& position, InputIterator begin, InputIterator end)
    {
        List temp(begin, end, this->GetAllocator());
        Splice(position, std::move(temp));
    }

    IteratorAlias Erase(IteratorAlias& position)
    {
        IteratorAlias returnIterator = IteratorAlias(position.NodeBase->PointerNext);
        EraseInternal(position);
        return returnIterator;
    }

    IteratorAlias Erase(IteratorAlias& begin, IteratorAlias& end)
    {
        IteratorAlias it = begin;
        while (it != end)
        {
            it = Erase(it);
        }
        return end;
    }

    void Remove(const ValueTypeAlias& value)
    {
        IteratorAlias begin = GetBegin();
        IteratorAlias end = GetEnd();
        IteratorAlias extra = end;

        while (begin != end)
        {
            IteratorAlias next = begin;
            ++next;

            if (*begin == value)
            {
                if (std::addressof(*begin) != std::addressof(value))
                {
                    this->EraseInternal(begin);
                }
                else
                {
                    extra = begin;
                }
            }

            begin = next;
        }

        if (extra != end)
        {
            this->EraseInternal(extra);
        }
    }

    template <typename Predicate> void RemoveIf(Predicate predicate)
    {
        IteratorAlias begin = GetBegin();
        IteratorAlias end = GetEnd();
        while (begin != end)
        {
            IteratorAlias next = begin;
            ++next;

            if (predicate(*begin))
            {
                EraseInternal(begin);
            }

            begin = next;
        }
    }

    void Merge(List&& list)
    {
        if (this != &list)
        {
            IteratorAlias begin1 = GetBegin();
            IteratorAlias end1 = GetEnd();
            IteratorAlias begin2 = list.GetBegin();
            IteratorAlias end2 = list.GetEnd();

            while (begin1 != end1 && begin2 != end2)
            {
                if (*begin2 < *begin1)
                {
                    IteratorAlias next = begin2;
                    this->TransferInternal(begin1, begin2, ++next);
                    begin2 = next;
                }
                else
                {
                    ++begin1;
                }
            }

            if (begin2 != end2)
            {
                this->TransferInternal(end1, begin2, end2);
            }
        }
    }

    void Merge(List& list)
    {
        Merge(std::move(list));
    }

    template <typename Compare> void Merge(List&& list, Compare compare)
    {
        if (this != &list)
        {
            IteratorAlias begin1 = GetBegin();
            IteratorAlias end1 = GetEnd();
            IteratorAlias begin2 = list.GetBegin();
            IteratorAlias end2 = list.GetEnd();

            while (begin1 != end1 && begin2 != end2)
            {
                if (compare(*begin2, *begin1))
                {
                    IteratorAlias next = begin2;
                    ++next;
                    this->TransferInternal(begin1, begin2, next);
                    begin2 = next;
                }
                else
                {
                    ++begin1;
                }
            }

            if (begin2 != end2)
            {
                this->TransferInternal(end1, begin2, end2);
            }
        }
    }

    template <typename Compare> void Merge(List& list, Compare compare)
    {
        Merge(std::move(list), compare);
    }

    void Sort()
    {
        this->Sort(std::less<ValueTypeAlias>());
    }

    template <typename TCompare> void Sort(TCompare compare)
    {
        // IteratorAlias iLeft;
        // IteratorAlias iRight;
        // IteratorAlias iNode;
        // IteratorAlias iHead;
        // IteratorAlias iTail;

        // int listSize = 1;
        // int numberOfMerges;
        // int leftSize;
        // int rightSize;

        // while (true)
        // {
        //     iLeft = GetBegin();
        //     iHead = GetEnd();
        //     iTail = GetEnd();

        //     numberOfMerges = 0;

        //     while (iLeft != GetEnd())
        //     {
        //         ++numberOfMerges;
        //         iRight = iLeft;
        //         leftSize = 0;

        //         for (int i = 0; i < listSize; ++i)
        //         {
        //             ++leftSize;
        //             ++iRight;

        //             if (iRight == GetEnd())
        //             {
        //                 break;
        //             }
        //         }

        //         rightSize = listSize;

        //         while (leftSize > 0 || (rightSize > 0 && iRight != GetEnd()))
        //         {
        //             if (leftSize == 0)
        //             {
        //                 iNode = iRight;
        //                 ++iRight;
        //                 --rightSize;
        //             }

        //             else if (rightSize == 0 || iRight == iTail)
        //             {
        //                 iNode = iLeft;
        //                 ++iLeft;
        //                 --leftSize;
        //             }

        //             else if (!compare(*iLeft, *iRight))
        //             {
        //                 iNode = iLeft;
        //                 ++iLeft;
        //                 --leftSize;
        //             }
        //             else
        //             {
        //                 iNode = iRight;
        //                 ++iRight;
        //                 --rightSize;
        //             }

        //             if (iHead == GetEnd())
        //             {
        //                 iHead.NodeBase.PointerNext = iNode.NodeBase;
        //                 iNode.NodeBase.PointerPrev = &iHead.NodeBase;
        //                 iHead = iNode;
        //                 iTail = iNode;
        //             }
        //             else
        //             {
        //                 iTail.NodeBase.PointerNext = iNode.NodeBase;
        //                 iNode.NodeBase.PointerPrev = &iTail.NodeBase;
        //                 iTail = iNode;
        //             }

        //         }

        //         iLeft = iTail;
        //     }
        // }
    }

    void Reverse() noexcept
    {
        this->LinkedListCore.NodeBase.ReverseAfterInternal();
    }

    void Swap(List& list) noexcept
    {
        ::DSALibraries::Containers::ListNodeBase::Swap(this->LinkedListCore.NodeBase, list.LinkedListCore.NodeBase);
    }

    void Clear() noexcept
    {
        this->ClearInternal();
        this->InitInternal();
    }

  private:
    void DefaultInitializeInternal(SizeTypeAlias size)
    {
        SizeTypeAlias i = 0;
        try
        {
            for (; i < size; ++i)
            {
                EmplaceBack();
            }
        }
        catch (...)
        {
            for (; i > 0; --i)
            {
                PopBack();
            }
            throw;
        }
    }

    /**
     * @brief Khởi tạo danh sách từ một giá trị cụ thể
     *
     * @param size
     * @param value
     */
    void FillInitializeInternal(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        for (; size > 0; --size)
        {
            PushBack(value);
        }
    }

    /**
     * @brief Khởi tạo danh sách từ một phạm vi [begin, end)
     *
     * @tparam InputIterator
     * @param begin
     * @param end
     */
    void RangeInitalizeInternal(IteratorAlias begin, IteratorAlias end)
    {

        for (; begin != end; ++begin)
        {
            PushBack(*begin);
        }
    }

    /**
     * @brief Assign n giá trị value vào danh sách từ vị trí đầu tiên [begin, end) hoặc kết thúc [end, end)
     *
     * @param size
     * @param value
     */
    void FillAssignInternal(SizeTypeAlias size, const ValueTypeAlias& value)
    {
        IteratorAlias begin = GetBegin();

        for (; begin != GetEnd() && size > 0; ++begin, --size)
        {
            *begin = value;
        }

        if (size > 0)
        {
            InsertAfter(GetEnd(), size, value);
        }
        else
        {
            Erase(begin, GetEnd());
        }
    }

    /**
     * @brief Gán giá trị từ vào danh sách từ vị trí đầu tiên [begin, end) hoặc kết thúc [end, end)
     *
     * @tparam InputIterator
     * @param begin
     * @param end
     */
    template <typename InputIterator> void RangeAssignInternal(InputIterator begin, InputIterator end)
    {
        IteratorAlias beginIterator = GetBegin();
        IteratorAlias endIterator = GetEnd();

        for (; beginIterator != endIterator && begin != end; ++beginIterator, ++begin)
        {
            *beginIterator = *begin;
        }

        if (begin == end)
        {
            Erase(beginIterator, endIterator);
        }
        else
        {
            InsertAfter(endIterator, begin, end);
        }
    }

    void InsertInternal(IteratorAlias position, const ValueTypeAlias& value)
    {
        NodeAlias* newNode = this->CreateNodeInternal(value);
        newNode->HookInternal(position.NodeBase);
        return;
    }

    template <typename... Args> void InsertInternal(IteratorAlias position, Args&&... args)
    {
        NodeAlias* newNode = this->CreateNodeInternal(std::forward<Args>(args)...);
        newNode->HookInternal(position.NodeBase);
        return;
    }

    void EraseInternal(IteratorAlias position)
    {
        position.NodeBase->UnhookInternal();

        NodeAlias* node = static_cast<NodeAlias*>(position.NodeBase);

        this->GetNodeAllocatorInternal().destroy(node->GetData());

        this->PutNodeInternal(node);
    }

    void TransferInternal(IteratorAlias position, IteratorAlias begin, IteratorAlias end)
    {
        position.NodeBase->TransferAfterInternal(begin.NodeBase, end.NodeBase);
    }
};

template <typename T, typename Alloc>
inline bool operator==(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) noexcept
{
    typedef typename List<T, Alloc>::ConstIteratorAlias ConstIteratorAlias;

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
inline bool operator!=(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) noexcept
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc> inline void Swap(List<T, Alloc>& lhs, List<T, Alloc>& rhs) noexcept
{
    lhs.Swap(rhs);
}
} // namespace DSALibraries::Containers

#endif