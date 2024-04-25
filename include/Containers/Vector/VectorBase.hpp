#ifndef DSA_LIBRARIES_VECTOR_BASE_HPP
#define DSA_LIBRARIES_VECTOR_BASE_HPP
#include "../../Utilities/Algorithm.hpp"
#include "../../Utilities/Memory.hpp"
#include "VectorExceptions.hpp"
#include <cstddef>
#include <iterator>
namespace DSALibraries::Containers
{
// class Base
// {
//   private:
//     const size_t _capacity;

//   public:
//     using SizeTypeAlias = size_t;

//     SizeTypeAlias Capacity() const
//     {
//         return _capacity;
//     }

//     SizeTypeAlias MaxSize() const
//     {
//         return Capacity();
//     }

//   protected:
//     explicit Base(SizeTypeAlias capacity) : _capacity(capacity)
//     {
//     }

//     ~Base()
//     {
//     }
// };

// template <typename TIterator> class IVector : public Base
// {
//   public:
//     using ValueTypeAlias = TIterator;
//     using ReferenceTypeAlias = TIterator &;
//     using ConstReferenceTypeAlias = const TIterator &;
//     using PointerTypeAlias = TIterator *;
//     using ConstPointerTypeAlias = const TIterator *;
//     using IteratorAlias = TIterator *;
//     using ConstIteratorAlias = const TIterator *;
//     using ReverseIteratorAlias = std::reverse_iterator<IteratorAlias>;
//     using ConstReverseIteratorAlias = std::reverse_iterator<ConstIteratorAlias>;
//     using SizeTypeAlias = typename Base::SizeTypeAlias;
//     using DifferenceTypeAlias = std::ptrdiff_t;

//     // Adaptive alias
//     using value_type = ValueTypeAlias;
//     using reference = ReferenceTypeAlias;
//     using const_reference = ConstReferenceTypeAlias;
//     using pointer = PointerTypeAlias;
//     using const_pointer = ConstPointerTypeAlias;
//     using iterator = IteratorAlias;
//     using const_iterator = ConstIteratorAlias;
//     using reverse_iterator = ReverseIteratorAlias;
//     using const_reverse_iterator = ConstReverseIteratorAlias;
//     using size_type = SizeTypeAlias;
//     using difference_type = std::ptrdiff_t;

//   public:
//     IteratorAlias GetBegin() const
//     {
//         return _pStart;
//     }
//     ConstIteratorAlias GetConstBegin() const
//     {
//         return _pStart;
//     }
//     IteratorAlias GetEnd() const
//     {
//         return _pEnd;
//     }
//     ConstIteratorAlias GetConstEnd() const
//     {
//         return _pEnd;
//     }
//     ReverseIteratorAlias GetReverseBegin() const
//     {
//         return ReverseIteratorAlias(_pEnd);
//     }
//     ConstReverseIteratorAlias GetConstReverseBegin() const
//     {
//         return ConstReverseIteratorAlias(_pEnd);
//     }
//     ReverseIteratorAlias GetReverseEnd() const
//     {
//         return ReverseIteratorAlias(_pStart);
//     }
//     ConstReverseIteratorAlias GetConstReverseEnd() const
//     {
//         return ConstReverseIteratorAlias(_pStart);
//     }
//     constexpr SizeTypeAlias GetSize() const
//     {
//         return static_cast<SizeTypeAlias>(_pEnd - _pStart);
//     }

//     void Resize(SizeTypeAlias newSize)
//     {
//         Resize(newSize, ValueTypeAlias());
//     }
//     void Resize(SizeTypeAlias newSize, ConstReferenceTypeAlias value)
//     {
//         const SizeTypeAlias currentSize = GetSize();
//         SizeTypeAlias delta = (currentSize < newSize) ? newSize - currentSize : currentSize - newSize;

//         if (currentSize < newSize)
//         {
//             Utilities::UninitializedFillElements(_pEnd, delta, value);
//         }
//         else
//         {
//             Utilities::DestroyElements(_pEnd - delta, delta);
//         }

//         _pEnd = _pStart + newSize;
//     }

//     void UnInitializeWhenResize(SizeTypeAlias newSize)
//     {
//         _pEnd = _pStart + newSize;
//     }

//     void Reserve(SizeTypeAlias newCapacity)
//     {
//         throw std::runtime_error("Not implemented");
//     }

//     ReferenceTypeAlias operator[](SizeTypeAlias index)
//     {
//         return _pStart[index];
//     }

//     ConstReferenceTypeAlias operator[](SizeTypeAlias index) const
//     {
//         return _pStart[index];
//     }

//     ReferenceTypeAlias At(SizeTypeAlias index)
//     {
//         if (index >= GetSize())
//         {
//             throw VectorOutOfBoundsException(__FILE__, __LINE__);
//         }
//         return _pStart[index];
//     }

//     ConstReferenceTypeAlias At(SizeTypeAlias index) const
//     {
//         if (index >= GetSize())
//         {
//             throw VectorOutOfBoundsException(__FILE__, __LINE__);
//         }
//         return _pStart[index];
//     }

//     ReferenceTypeAlias GetItemFront()
//     {
//         return *_pStart;
//     }

//     ConstReferenceTypeAlias GetItemFront() const
//     {
//         return *_pStart;
//     }

//     ReferenceTypeAlias GetItemBack()
//     {
//         return *(_pEnd - 1);
//     }

//     ConstReferenceTypeAlias GetItemBack() const
//     {
//         return *(_pEnd - 1);
//     }

//     PointerTypeAlias GetData()
//     {
//         return _pStart;
//     }

//     ConstPointerTypeAlias GetData() const
//     {
//         return _pStart;
//     }

//     template <typename TIterator>
//     typename std::enable_if<!std::is_integral<TIterator>::value, void>::type Assign(TIterator first, TIterator last)
//     {
//         auto size = std::distance(first, last);

//         if (static_cast<SizeTypeAlias>(size) > Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         Initialize();

//         _pEnd = Utilities::UninitializedCopy(first, last, _pStart);
//     }

//     void Assign(SizeTypeAlias count, ConstReferenceTypeAlias value)
//     {
//         if (count > Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         Initialize();
//         _pEnd = Utilities::UninitializedFillElements(_pStart, count, value);
//     }

//     void Clear()
//     {
//         Initialize();
//     }

//     void Fill(ConstReferenceTypeAlias value)
//     {
//         Utilities::Fill(_pStart, _pEnd, value);
//     }

//     void PushBack(ConstReferenceTypeAlias value)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         CreateBack(value);
//     }

//     void PushBack(ValueTypeAlias &&value)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         CreateBack(std::move(value));
//     }

//     template <typename... Args> ReferenceTypeAlias EmplaceBack(Args &&...args)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         ::new (static_cast<void *>(_pEnd)) ValueTypeAlias(std::forward<Args>(args)...);
//         ++_pEnd;
//         return GetItemBack();
//     }

//     IteratorAlias Insert(ConstIteratorAlias position, ConstReferenceTypeAlias value)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         IteratorAlias pos = const_cast<IteratorAlias>(position);

//         if (pos == GetConstEnd())
//         {
//             CreateBack(value);
//         }
//         else
//         {
//             CreateBack(GetItemBack());
//             Utilities::MoveBackward(pos, _pEnd - 2, _pEnd - 1);
//             *pos = value;
//         }

//         return pos;
//     }

//     IteratorAlias Insert(ConstIteratorAlias position, ValueTypeAlias &&value)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         IteratorAlias pos = const_cast<IteratorAlias>(position);

//         if (pos == GetConstEnd())
//         {
//             CreateBack(std::move(value));
//         }
//         else
//         {
//             CreateBack(GetItemBack());
//             Utilities::MoveBackward(pos, _pEnd - 2, _pEnd - 1);
//             *pos = std::move(value);
//         }

//         return pos;
//     }

//     template <typename... Args> IteratorAlias Emplace(ConstIteratorAlias position, Args &&...args)
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         IteratorAlias pos = const_cast<IteratorAlias>(position);
//         void *p;

//         if (pos == GetConstEnd())
//         {
//             p = static_cast<void *>(_pEnd++);
//         }
//         else
//         {
//             p = std::addressof(*pos);
//             CreateBack(GetItemBack());
//             Utilities::MoveBackward(pos, _pEnd - 2, _pEnd - 1);
//             (*pos).~ValueTypeAlias();
//         }

//         ::new (p) ValueTypeAlias(std::forward<Args>(args)...);

//         return pos;
//     }

//     void Insert(ConstIteratorAlias position, SizeTypeAlias count, ConstReferenceTypeAlias value)
//     {
//         if (GetSize() + count > Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         IteratorAlias pos = const_cast<IteratorAlias>(position);

//         SizeTypeAlias insertElements = count;
//         SizeTypeAlias insertBegin = std::distance(GetConstBegin(), pos);
//         SizeTypeAlias insertEnd = insertBegin + count;

//         SizeTypeAlias copyOldElements;
//         SizeTypeAlias constructOldElements;
//         IteratorAlias insertPos;

//         if (insertEnd > GetSize())
//         {
//             copyOldElements = 0;
//             constructOldElements = GetSize() - insertBegin;
//             insertPos = _pStart + insertEnd;
//         }
//         else
//         {
//             copyOldElements = GetSize() - insertBegin - insertElements;
//             constructOldElements = insertElements;
//             insertPos = _pEnd;
//         }

//         SizeTypeAlias copyNewElements = constructOldElements;
//         SizeTypeAlias constructNewElements = insertElements - copyNewElements;

//         Utilities::UnintializedMove(_pEnd - constructOldElements, _pEnd, insertPos);

//         Utilities::MoveBackward(_pStart + insertBegin, _pStart + insertBegin + copyOldElements,
//                                 _pStart + insertEnd + copyOldElements);

//         Utilities::UninitializedFillElements(_pEnd, constructNewElements, value);

//         Utilities::FillElements(_pStart + insertBegin, copyNewElements, value);

//         _pEnd += count;
//     }

//     template <class TIterator>
//     void Insert(ConstIteratorAlias position, TIterator first, TIterator last,
//                 typename std::enable_if<!std::is_integral<TIterator>::value, int>::type = 0)
//     {
//         SizeTypeAlias count = std::distance(first, last);

//         if (GetSize() + count > Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         SizeTypeAlias insertElements = count;
//         SizeTypeAlias insertBegin = std::distance(GetConstBegin(), position);
//         SizeTypeAlias insertEnd = insertBegin + count;

//         SizeTypeAlias copyOldElements;
//         SizeTypeAlias constructOldElements;
//         IteratorAlias insertPos;

//         if (insertEnd > GetSize())
//         {
//             copyOldElements = 0;
//             constructOldElements = GetSize() - insertBegin;
//             insertPos = _pStart + insertEnd;
//         }
//         else
//         {
//             copyOldElements = GetSize() - insertBegin - insertElements;
//             constructOldElements = insertElements;
//             insertPos = _pEnd;
//         }

//         SizeTypeAlias copyNewElements = constructOldElements;
//         SizeTypeAlias constructNewElements = insertElements - copyNewElements;

//         Utilities::UnintializedMove(_pEnd - constructOldElements, _pEnd, insertPos);

//         Utilities::MoveBackward(_pStart + insertBegin, _pStart + insertBegin + copyOldElements,
//                                 _pStart + insertEnd + copyOldElements);

//         Utilities::UninitializedCopy(first + last, first + constructNewElements + copyNewElements, _pEnd);

//         Utilities::Copy(first, first + copyNewElements, _pStart + insertBegin);

//         _pEnd += count;
//     }

//     IteratorAlias Erase(IteratorAlias position)
//     {
//         if (position == GetEnd())
//         {
//             throw VectorOutOfBoundsException(__FILE__, __LINE__);
//         }

//         Utilities::Move(position + 1, _pEnd, position);

//         DestroyBack();

//         return position;
//     }

//     IteratorAlias Erase(ConstIteratorAlias position)
//     {
//         return Erase(const_cast<IteratorAlias>(position));
//     }

//     IteratorAlias Erase(ConstIteratorAlias first, ConstIteratorAlias last)
//     {
//         IteratorAlias firstPos = const_cast<IteratorAlias>(first);
//         IteratorAlias lastPos = const_cast<IteratorAlias>(last);

//         if (first == GetBegin() && last == GetEnd())
//         {
//             Clear();
//         }
//         else
//         {
//             Utilities::Move(lastPos, _pEnd, firstPos);
//             Utilities::DestroyRange(lastPos, _pEnd);
//             _pEnd = lastPos;
//         }

//         return firstPos;
//     }

//     ReferenceTypeAlias EmplaceBack()
//     {
//         if (GetSize() == Capacity())
//         {
//             throw VectorFullException(__FILE__, __LINE__);
//         }

//         ::new (static_cast<void *>(_pEnd)) ValueTypeAlias();
//         ++_pEnd;
//         return GetItemBack();
//     }

//     void PopBack()
//     {
//         if (GetSize() == 0)
//         {
//             throw VectorEmptyException(__FILE__, __LINE__);
//         }

//         DestroyBack();
//     }

//     IVector &operator=(const IVector &rhs)
//     {
//         if (this != &rhs)
//         {
//             this->Assign(rhs.GetConstBegin(), rhs.GetConstEnd());
//         }

//         return *this;
//     }

//     IVector &operator=(IVector &&rhs)
//     {
//         if (this != &rhs)
//         {
//             Clear();

//             IteratorAlias first = rhs.GetBegin();

//             while (first != rhs.GetEnd())
//             {
//                 this->PushBack(std::move(*first));
//                 ++first;
//             }

//             rhs.Initialize();
//         }

//         return *this;
//     }

//     SizeTypeAlias AvailableSpace() const
//     {
//         return Capacity() - GetSize();
//     }

//     bool IsEmpty() const
//     {
//         return _pStart == _pEnd;
//     }

//     bool IsFull() const
//     {
//         return GetSize() == Capacity();
//     }

//     virtual void Repair() = 0;

//   private:
//     PointerTypeAlias _pStart;
//     PointerTypeAlias _pEnd;

//   protected:
//     IVector(TIterator *pBuffer, SizeTypeAlias capacity) : Base(capacity), _pStart(pBuffer), _pEnd(pBuffer)
//     {
//     }

//     void Initialize()
//     {
//         Utilities::DestroyRange(_pStart, _pEnd);
//         _pEnd = _pStart;
//     }

//     void RepairBuffer(TIterator *pBuffer)
//     {
//         uintptr_t length = _pEnd - _pStart;
//         _pStart = pBuffer;
//         _pEnd = pBuffer + length;
//     }

//   private:
//     void CreateBack()
//     {
//         Utilities::CreateValueAt(_pEnd);

//         ++_pEnd;
//     }

//     void CreateBack(ConstReferenceTypeAlias value)
//     {
//         Utilities::CreateValueAt(_pEnd, value);

//         ++_pEnd;
//     }

//     void CreateBack(ValueTypeAlias &&value)
//     {
//         Utilities::CreateValueAt(_pEnd, std::move(value));

//         ++_pEnd;
//     }

//     void DestroyBack()
//     {
//         --_pEnd;

//         Utilities::DestroyAt(_pEnd);
//     }

//     IVector(const IVector &) = delete;
// };

// template <typename TIterator> bool operator==(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     if (lhs.GetSize() != rhs.GetSize())
//     {
//         return false;
//     }

//     for (size_t i = 0; i < lhs.GetSize(); ++i)
//     {
//         if (lhs[i] != rhs[i])
//         {
//             return false;
//         }
//     }

//     return true;
// }

// template <typename TIterator> bool operator!=(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     return !(lhs == rhs);
// }

// template <typename TIterator> bool operator<(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     return std::lexicographical_compare(lhs.GetConstBegin(), lhs.GetConstEnd(), rhs.GetConstBegin(),
//     rhs.GetConstEnd());
// }

// template <typename TIterator> bool operator>(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     return rhs < lhs;
// }

// template <typename TIterator> bool operator<=(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     return !(rhs < lhs);
// }

// template <typename TIterator> bool operator>=(const IVector<TIterator> &lhs, const IVector<TIterator> &rhs)
// {
//     return !(lhs < rhs);
// }
template <typename T, typename Alloc> struct VectorBase
{
  protected:
    using TypeAllocatorTraits = std::allocator_traits<Alloc>;
    using TpAllocatorTypeAlias = typename TypeAllocatorTraits::template rebind_alloc<T>;
    using PointerTypeAlias = typename TypeAllocatorTraits::pointer;

    struct VectorCore : public TpAllocatorTypeAlias
    {
        PointerTypeAlias PointerStart;
        PointerTypeAlias PointerFinish;
        PointerTypeAlias PointerEndOfStorage;

        constexpr VectorCore() : TpAllocatorTypeAlias(), PointerStart(), PointerFinish(), PointerEndOfStorage()
        {
        }

        constexpr VectorCore(TpAllocatorTypeAlias const &allocator)
            : TpAllocatorTypeAlias(allocator), PointerStart(), PointerFinish(), PointerEndOfStorage()
        {
        }

        constexpr VectorCore(TpAllocatorTypeAlias &&allocator)
            : TpAllocatorTypeAlias(std::move(allocator)), PointerStart(), PointerFinish(), PointerEndOfStorage()
        {
        }

        constexpr void CopyDataInternal(VectorCore const &other) noexcept
        {
            this->PointerStart = other.PointerStart;
            this->PointerFinish = other.PointerFinish;
            this->PointerEndOfStorage = other.PointerEndOfStorage;
        }

        void SwapDataInternal(VectorCore &other) noexcept
        {
            VectorCore temp;
            temp.CopyDataInternal(*this);
            this->CopyDataInternal(other);
            other.CopyDataInternal(temp);
        }
    };

  protected:
    constexpr void CreateStorageInternal(size_t size)
    {
        this->VectorData.PointerStart = this->AllocateInternal(size);
        this->VectorData.PointerFinish = this->VectorData.PointerStart;
        this->VectorData.PointerEndOfStorage = this->VectorData.PointerStart + size;
    }

  public:
    using AllocatorTypeAlias = Alloc;
    VectorCore VectorData;

    constexpr TpAllocatorTypeAlias &GetTpAllocator() noexcept
    {
        return *static_cast<TpAllocatorTypeAlias *>(&this->VectorData);
    }

    constexpr const TpAllocatorTypeAlias &GetTpAllocator() const noexcept
    {
        return *static_cast<TpAllocatorTypeAlias const *>(&this->VectorData);
    }

    constexpr AllocatorTypeAlias GetAllocator() noexcept
    {
        return AllocatorTypeAlias(GetTpAllocator());
    }

    constexpr AllocatorTypeAlias GetAllocator() const noexcept
    {
        return AllocatorTypeAlias(GetTpAllocator());
    }

    constexpr VectorBase() : VectorData()
    {
    }

    constexpr VectorBase(AllocatorTypeAlias const &allocator) noexcept : VectorData(allocator)
    {
    }

    constexpr VectorBase(size_t size) : VectorData()
    {
        CreateStorageInternal(size);
    }

    constexpr VectorBase(size_t size, AllocatorTypeAlias const &allocator) : VectorData(allocator)
    {
        CreateStorageInternal(size);
    }

    constexpr VectorBase(size_t size, AllocatorTypeAlias &&allocator) : VectorData(std::move(allocator))
    {
        CreateStorageInternal(size);
    }

    constexpr VectorBase(TpAllocatorTypeAlias &&allocator) : VectorData(std::move(allocator))
    {
    }

    constexpr VectorBase(VectorBase &&other) noexcept : VectorData(std::move(other.GetTpAllocator()))
    {
        this->VectorData.SwapDataInternal(other.VectorData);
    }

    constexpr VectorBase(VectorBase &&other, AllocatorTypeAlias const &allocator) noexcept : VectorData(allocator)
    {
        if (this->GetTpAllocator() == other.GetTpAllocator())
        {
            this->VectorData.SwapDataInternal(other.VectorData);
        }
        else
        {
            size_t size = other.VectorData.PointerFinish - other.VectorData.PointerStart;
            CreateStorageInternal(size);
        }
    }

    constexpr PointerTypeAlias AllocateInternal(size_t size)
    {
        return size != 0 ? TypeAllocatorTraits::allocate(this->GetTpAllocator(), size) : PointerTypeAlias();
    }

    constexpr void DeallocateInternal(PointerTypeAlias p, size_t size)
    {
        if (p != nullptr)
        {
            TypeAllocatorTraits::deallocate(this->GetTpAllocator(), p, size);
        }
    }
};

} // namespace DSALibraries::Containers

#endif