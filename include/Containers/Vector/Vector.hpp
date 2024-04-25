/**
 * @file Vector.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DSA_LIBRARIES_VECTOR_HPP
#define DSA_LIBRARIES_VECTOR_HPP
#include "../../Utilities/Allocator.hpp"
#include "VectorBase.hpp"
#include "VectorExceptions.hpp"
#include "VectorIterator.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>

namespace DSALibraries::Containers
{
template <typename T, typename Alloc = DSALibraries::Utilities::Allocator<T>>
class Vector : protected VectorBase<T, Alloc>
{
  private:
    using AllocValueTypeAlias [[maybe_unused]] = typename Alloc::value_type;
    using Base = VectorBase<T, Alloc>;
    using TpAllocatorTypeAlias = typename Base::TpAllocatorTypeAlias;
    using AllocatorTraitsAlias = std::allocator_traits<TpAllocatorTypeAlias>;

  public:
    using ValueTypeAlias = T;
    using PointerAlias = Base::PointerTypeAlias;
    using ConstPointerAlias = typename AllocatorTraitsAlias::const_pointer;
    using ReferenceAlias = T &;
    using ConstReferenceAlias = const T &;
    using IteratorAlias = NormalVectorIter<PointerAlias, Vector>;
    using ConstIteratorAlias = NormalVectorIter<ConstPointerAlias, Vector>;
    using ReverseIteratorAlias = std::reverse_iterator<IteratorAlias>;
    using ConstReverseIteratorAlias = std::reverse_iterator<ConstIteratorAlias>;
    using SizeTypeAlias = size_t;
    using DifferenceTypeAlias = std::ptrdiff_t;
    using AllocatorTypeAlias = Alloc;

    using value_type [[maybe_unused]] = ValueTypeAlias;
    using pointer [[maybe_unused]] = PointerAlias;
    using const_pointer [[maybe_unused]] = ConstPointerAlias;
    using reference [[maybe_unused]] = ReferenceAlias;
    using const_reference [[maybe_unused]] = ConstReferenceAlias;
    using iterator [[maybe_unused]] = IteratorAlias;
    using const_iterator [[maybe_unused]] = ConstIteratorAlias;
    using reverse_iterator [[maybe_unused]] = ReverseIteratorAlias;
    using const_reverse_iterator [[maybe_unused]] = ConstReverseIteratorAlias;
    using size_type [[maybe_unused]] = SizeTypeAlias;
    using difference_type [[maybe_unused]] = DifferenceTypeAlias;

  protected:
    struct TemporaryValue
    {
        template <typename... Args> explicit constexpr TemporaryValue(Vector *pThis, Args &&...args) : _pThis(pThis)
        {
            AllocatorTraitsAlias::construct(_pThis->VectorData, GetPointerInternal(), std::forward<Args>(args)...);
        }

        ~TemporaryValue()
        {
            AllocatorTraitsAlias::destroy(_pThis->VectorData, GetPointerInternal());
        }

        constexpr ValueTypeAlias &GetValueInternal()
        {
            return _storage._value;
        }

      private:
        constexpr PointerAlias GetPointerInternal()
        {
            return std::addressof(_storage._value);
        }
        union Storage {
            constexpr Storage() noexcept : _buffer()
            {
            }
            constexpr ~Storage() noexcept
            {
            }
            Storage &operator=(const Storage &) = delete;
            unsigned char _buffer;
            T _value;
        };
        Vector *_pThis;
        Storage _storage;
    };

  public:
    constexpr Vector() noexcept : Base()
    {
    }

    constexpr explicit Vector(const AllocatorTypeAlias &alloc) : Base(alloc)
    {
    }

    constexpr explicit Vector(SizeTypeAlias size, const AllocatorTypeAlias &alloc = AllocatorTypeAlias())
        : Base(size, alloc)
    {
        this->Initialize(size);
    }

    constexpr Vector(SizeTypeAlias size, ConstReferenceAlias value,
                     const AllocatorTypeAlias &alloc = AllocatorTypeAlias())
        : Base(size, value, alloc)
    {
        this->FillInitialize(size, value);
    }

    constexpr Vector(const Vector &other)
        : Base(other.GetSize(), AllocatorTraitsAlias::select_on_container_copy_construction(other.GetAllocator()))
    {
        this->VectorData.PointerFinish =
            Utilities::UninitializedCopy(other.GetConstBegin(), other.GetConstEnd(), this->VectorData.PointerStart);
    }

    constexpr Vector(const Vector &vector, const AllocatorTypeAlias &alloc) : Base(vector.GetSize(), alloc)
    {
        this->VectorData.PointerFinish =
            Utilities::UninitializedCopy(vector.GetConstBegin(), vector.GetConstEnd(), this->VectorData.PointerStart);
    }

    Vector(Vector &&vector) = default;

  private:
    constexpr Vector(Vector &&other, const AllocatorTypeAlias &alloc, std::true_type) noexcept
        : Base(std::move(other), alloc)
    {
    }

    constexpr Vector(Vector &&other, const AllocatorTypeAlias &alloc, std::false_type) : Base(alloc)
    {
        if (alloc == other.GetTpAllocator())
        {
            this->VectorData.SwapDataInternal(other.VectorData);
        }
        else if (!other.IsEmpty())
        {
            this->CreateStorageInternal(other.GetSize());
            this->VectorData.PointerFinish =
                Utilities::UninitializedMove(other.GetBegin(), other.GetEnd(), this->VectorData.PointerStart);
            other.Clear();
        }
    }

  public:
    constexpr Vector(Vector &&other, const AllocatorTypeAlias &alloc = AllocatorTypeAlias()) noexcept
        : Vector(std::move(other), alloc, typename AllocatorTraitsAlias::is_always_equal{})
    {
    }

    constexpr Vector(std::initializer_list<ValueTypeAlias> initList,
                     const AllocatorTypeAlias &alloc = AllocatorTypeAlias())
        : Base(alloc)
    {
        this->RangeInitialize(initList.begin(), initList.end(), std::random_access_iterator_tag());
    }

    template <typename TInputIterator>
    constexpr Vector(TInputIterator first, TInputIterator last, const AllocatorTypeAlias &alloc = AllocatorTypeAlias())
        : Base(alloc)
    {
        typedef typename std::is_integral<TInputIterator>::type IntegralType;
        this->DefaultInitialize(first, last, IntegralType());
    }

    ~Vector()
    {
        Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);
    }

    constexpr Vector &operator=(const Vector &other)
    {
        if (std::addressof(other) != this)
        {
            if (AllocatorTraitsAlias::propagate_on_container_copy_assignment::value)
            {
                if (!AllocatorTraitsAlias::is_always_equal::value && this->GetTpAllocator() != other.GetTpAllocator())
                {
                    this->Clear();
                    this->DeallocateInternal(this->VectorData.PointerStart,
                                             this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);
                    this->VectorData.PointerStart = nullptr;
                    this->VectorData.PointerFinish = nullptr;
                    this->VectorData.PointerEndOfStorage = nullptr;
                }
                this->GetAllocator() =
                    AllocatorTraitsAlias::select_on_container_copy_construction(other.GetAllocator());
            }

            const SizeTypeAlias otherSize = other.GetSize();
            if (otherSize > this->GetCapacity())
            {
                PointerAlias temp = this->AllocateAndCopy(otherSize, other.GetBegin(), other.GetEnd());

                Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);
                this->DeallocateInternal(this->VectorData.PointerStart,
                                         this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);
                this->VectorData.PointerStart = temp;
                this->VectorData.PointerFinish = temp + otherSize;
            }
            else if (this->GetSize() >= otherSize)
            {
                Utilities::Destroy(Utilities::Copy(other.GetBegin(), other.GetEnd(), this->VectorData.PointerStart),
                                   this->VectorData.PointerFinish);
            }
            else
            {
                Utilities::Copy(other.VectorData.PointerStart, other.VectorData.PointerStart + this->GetSize(),
                                this->VectorData.PointerStart);

                Utilities::UninitializedCopy(other.VectorData.PointerStart + this->GetSize(),
                                             other.VectorData.PointerFinish, this->VectorData.PointerFinish);
            }

            this->VectorData.PointerFinish = this->VectorData.PointerStart + otherSize;
        }
        return *this;
    }

    constexpr Vector &operator=(Vector &&other) noexcept(
        AllocatorTraitsAlias::propagate_on_container_move_assignment::value)
    {
        constexpr bool moveStorage = AllocatorTraitsAlias::propagate_on_container_move_assignment::value ||
                                     AllocatorTraitsAlias::is_always_equal::value;

        this->MoveAssignInternal(std::move(other), std::integral_constant<bool, moveStorage>());
        return *this;
    }

    constexpr Vector &operator=(std::initializer_list<ValueTypeAlias> initList)
    {
        this->AssignInternal(initList.begin(), initList.end(), std::random_access_iterator_tag());
        return *this;
    }

    constexpr void Assign(SizeTypeAlias size, const ValueTypeAlias &value)
    {
        this->FillAssignInternal(size, value);
    }

    template <typename TInputIterator> constexpr void Assign(TInputIterator first, TInputIterator last)
    {
        using IteratorCategory = typename std::iterator_traits<TInputIterator>::iterator_category;
        this->AssignInternal(first, last, IteratorCategory());
    }

    constexpr void Assign(std::initializer_list<ValueTypeAlias> initList)
    {
        this->AssignInternal(initList.begin(), initList.end(), std::random_access_iterator_tag());
    }

    constexpr IteratorAlias GetBegin() noexcept
    {
        return IteratorAlias(this->VectorData.PointerStart);
    }

    constexpr ConstIteratorAlias GetConstBegin() const noexcept
    {
        return ConstIteratorAlias(this->VectorData.PointerStart);
    }

    constexpr IteratorAlias GetEnd() noexcept
    {
        return IteratorAlias(this->VectorData.PointerFinish);
    }

    constexpr ConstIteratorAlias GetConstEnd() const noexcept
    {
        return ConstIteratorAlias(this->VectorData.PointerFinish);
    }

    constexpr ReverseIteratorAlias GetReverseBegin() noexcept
    {
        return ReverseIteratorAlias(this->GetEnd());
    }

    constexpr ConstReverseIteratorAlias GetConstReverseBegin() const noexcept
    {
        return ConstReverseIteratorAlias(this->GetConstEnd());
    }

    constexpr ReverseIteratorAlias GetReverseEnd() noexcept
    {
        return ReverseIteratorAlias(this->GetBegin());
    }

    constexpr ConstReverseIteratorAlias GetConstReverseEnd() const noexcept
    {
        return ConstReverseIteratorAlias(this->GetConstBegin());
    }

    constexpr ReferenceAlias operator[](SizeTypeAlias index) noexcept
    {
        return this->VectorData.PointerStart[index];
    }

    constexpr ConstReferenceAlias operator[](SizeTypeAlias index) const noexcept
    {
        return this->VectorData.PointerStart[index];
    }

    constexpr ReferenceAlias At(SizeTypeAlias index)
    {
        if (index >= this->GetSize())
        {
            throw VectorOutOfBoundsException(__FILE__, __LINE__);
        }

        return (*this)[index];
    }

    [[nodiscard]] constexpr SizeTypeAlias GetSize() const noexcept
    {
        return SizeTypeAlias(this->VectorData.PointerFinish - this->VectorData.PointerStart);
    }

    [[nodiscard]] constexpr SizeTypeAlias GetMaxSize() const noexcept
    {
        const size_t diffMax = std::numeric_limits<std::ptrdiff_t>::max() / sizeof(ValueTypeAlias);
        const size_t allocMax = AllocatorTraitsAlias::max_size(this->GetAllocator());
        return (std::min)(diffMax, allocMax);
    }

    constexpr void ShrinkToFit()
    {
        ShrinkToFitInternal();
    }
    [[nodiscard]] constexpr SizeTypeAlias GetCapacity() const noexcept
    {
        return SizeTypeAlias(this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);
    }

    constexpr void Resize(SizeTypeAlias size)
    {
        if (size > this->GetSize())
        {
            this->DefaultAppendInternal(size - this->GetSize());
        }
        else if (size < this->GetSize())
        {
            this->EraseAtEndInternal(this->VectorData.PointerStart + size);
        }
    }

    constexpr void Resize(SizeTypeAlias size, ConstReferenceAlias value)
    {
        if (size > this->GetSize())
        {
            this->FillInsertInternal(this->GetEnd(), size - this->GetSize(), value);
        }
        else if (size < this->GetSize())
        {
            this->EraseAtEndInternal(this->GetBegin() + size);
        }
    }

    [[nodiscard]] constexpr bool IsEmpty() const noexcept
    {
        return this->GetConstBegin() == this->GetConstEnd();
    }

    constexpr ReferenceAlias GetItemFront() noexcept
    {
        return *this->GetBegin();
    }

    [[nodiscard]] constexpr ConstReferenceAlias GetItemFront() const noexcept
    {
        return *this->GetConstBegin();
    }

    constexpr ReferenceAlias GetItemBack() noexcept
    {
        return *(this->GetEnd() - 1);
    }

    [[nodiscard]] constexpr ConstReferenceAlias GetItemBack() const noexcept
    {
        return *(this->GetConstEnd() - 1);
    }

    constexpr void PopBack() noexcept
    {
        if (this->IsEmpty())
        {
            throw VectorEmptyException(__FILE__, __LINE__);
        }
        --this->VectorData.PointerFinish;
        AllocatorTraitsAlias::destroy(this->VectorData, this->VectorData.PointerFinish);
    }

    constexpr void Reverse(SizeTypeAlias size) noexcept
    {
        if (size > this->GetMaxSize())
        {
            throw VectorOutOfBoundsException(__FILE__, __LINE__);
        }

        if (this->GetCapacity() < size)
        {
            const SizeTypeAlias oldSize = this->GetSize();
            PointerAlias temp;

            temp = this->AllocateAndCopy(size, this->VectorData.PointerStart, this->VectorData.PointerFinish);
            Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);

            this->DeallocateInternal(this->VectorData.PointerStart,
                                     this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);

            this->VectorData.PointerStart = temp;
            this->VectorData.PointerFinish = temp + oldSize;
            this->VectorData.PointerEndOfStorage = temp + size;
        }
    }

    [[nodiscard]] constexpr PointerAlias GetData() noexcept
    {
        return this->GetDataPointerInternal(this->VectorData.PointerStart);
    }

    [[nodiscard]] constexpr ConstPointerAlias GetData() const noexcept
    {
        return this->GetDataPointerInternal(this->VectorData.PointerStart);
    }

    constexpr void PushBack(const ValueTypeAlias &value)
    {
        if (this->VectorData.PointerFinish != this->VectorData.PointerEndOfStorage)
        {
            AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish, value);
            ++this->VectorData.PointerFinish;
        }
        else
        {
            this->ReallocateAppendInternal(value);
        }
    }

    constexpr void PushBack(ValueTypeAlias &&value)
    {
        this->EmplaceBack(std::move(value));
    }

    template <typename... Args> constexpr ReferenceAlias EmplaceBack(Args &&...args)
    {
        if (this->VectorData.PointerFinish != this->VectorData.PointerEndOfStorage)
        {
            AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish,
                                            std::forward<Args>(args)...);
            ++this->VectorData.PointerFinish;
        }
        else
        {
            this->ReallocateAppendInternal(std::forward<Args>(args)...);
        }
        return this->GetItemBack();
    }

    template <typename... Args> constexpr IteratorAlias Emplace(ConstIteratorAlias position, Args &&...args)
    {
        return this->DefaultEmplaceInternal(position, std::forward<Args>(args)...);
    }

    constexpr IteratorAlias Insert(ConstIteratorAlias position, const ValueTypeAlias &value)
    {
        const SizeTypeAlias offset = position - this->GetBegin();
        if (this->VectorData.PointerFinish != this->VectorData.PointerEndOfStorage)
        {
            if (position == this->GetEnd())
            {
                AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish, value);
                ++this->VectorData.PointerFinish;
            }
            else
            {
                const auto pos = this->GetBegin() + (position - this->GetConstBegin());
                TemporaryValue temp(this, value);
                this->DefaultInsertInternal(pos, std::move(temp.GetValueInternal()));
            }
        }
        else
        {
            this->ReallocateInsertInternal(this->GetBegin() + (position - this->GetConstBegin()), value);
        }
        return IteratorAlias(this->VectorData.PointerStart + offset);
    }

    constexpr IteratorAlias Insert(ConstIteratorAlias position, ValueTypeAlias &&value)
    {
        return this->InsertRvalueInternal(position, std::move(value));
    }

    constexpr IteratorAlias Insert(ConstIteratorAlias position, std::initializer_list<ValueTypeAlias> initList)
    {
        auto offset = position - this->GetBegin();
        this->RangeInsertInternal(this->GetBegin() + offset, initList.begin(), initList.end(),
                                  std::random_access_iterator_tag());
        return this->GetBegin() + offset;
    }

    constexpr IteratorAlias Insert(ConstIteratorAlias position, SizeTypeAlias size, const ValueTypeAlias &value)
    {
        DifferenceTypeAlias offset = position - this->GetBegin();
        this->FillInsertInternal(this->GetBegin() + offset, size, value);
        return this->GetBegin() + offset;
    }

    template <
        typename TInputIterator,
        typename = std::enable_if<std::is_convertible<typename std::iterator_traits<TInputIterator>::iterator_category,
                                                      std::input_iterator_tag>::value>::type>
    constexpr IteratorAlias Insert(ConstIteratorAlias position, TInputIterator first, TInputIterator last)
    {
        DifferenceTypeAlias offset = position - this->GetBegin();
        using IteratorCategory = typename std::iterator_traits<TInputIterator>::iterator_category;
        this->RangeInsertInternal(this->GetBegin() + offset, first, last, IteratorCategory());
        return this->GetBegin() + offset;
    }

    constexpr IteratorAlias Erase(ConstIteratorAlias position)
    {
        return this->EraseInternal(this->GetBegin() + (position - this->GetConstBegin()));
    }

    constexpr IteratorAlias Erase(ConstIteratorAlias first, ConstIteratorAlias last)
    {
        const auto begin = this->GetBegin();
        const auto constBegin = this->GetConstBegin();
        return this->EraseInternal(begin + (first - constBegin), begin + (last - constBegin));
    }

    constexpr void Swap(Vector &other) noexcept
    {
        this->VectorData.SwapDataInternal(other.VectorData);
        if (AllocatorTraitsAlias::propagate_on_container_swap::value)
        {
            std::swap(this->GetAllocator(), other.GetAllocator());
        }
    }
    constexpr void Clear() noexcept
    {
        this->EraseAtEndInternal(this->VectorData.PointerStart);
    }

  protected:
    constexpr void FillInitialize(SizeTypeAlias size, ConstReferenceAlias value)
    {
        this->VectorData.PointerFinish =
            Utilities::UninitializedFillElements(this->VectorData.PointerStart, size, value);
    }

    template <typename TInputIterator>
    constexpr void RangeInitialize(TInputIterator first, TInputIterator last, std::input_iterator_tag)
    {
        try
        {
            for (; first != last; ++first)
            {
                this->EmplaceBack(*first);
            }
        }
        catch (...)
        {
            this->Clear();
            throw;
        }
    }

    template <typename TInputIterator>
    void DefaultInitialize(TInputIterator first, TInputIterator last, std::false_type)
    {
        using IteratorCategory = typename std::iterator_traits<TInputIterator>::iterator_category;
        this->RangeInitialize(first, last, IteratorCategory());
    }

    template <typename TInteger> void DefaultInitialize(TInteger size, TInteger value, std::true_type)
    {
        this->VectorData.PointerStart = this->AllocateInternal(static_cast<SizeTypeAlias>(size));
        this->VectorData.PointerEndOfStorage = this->VectorData.PointerStart + static_cast<SizeTypeAlias>(size);
        this->FillInitialize(static_cast<SizeTypeAlias>(size), value);
    }

    constexpr void Initialize(SizeTypeAlias size)
    {
        this->VectorData.PointerFinish =
            Utilities::UnInitializedDefaultConstructElements(this->VectorData.PointerStart, size);
    }

    constexpr void MoveAssignInternal(Vector &&other, std::true_type)
    {
        Vector temp(this->GetAllocator());
        this->VectorData.SwapDataInternal(temp.VectorData);
        this->VectorData.SwapDataInternal(other.VectorData);
        this->GetAllocator() = std::move(temp.GetAllocator());
    }

    constexpr void MoveAssignInternal(Vector &&other, std::false_type)
    {
        if (other.GetTpAllocator() == this->GetTpAllocator())
        {
            this->MoveAssignInternal(std::move(other), std::true_type());
        }
        else
        {
            this->Assign(other.GetBegin(), other.GetEnd());
            other.Clear();
        }
    }

    template <typename TInputIterator>
    constexpr void AssignInternal(TInputIterator first, TInputIterator last, std::input_iterator_tag)
    {
        PointerAlias current(this->VectorData.PointerStart);
        for (; first != last && current != this->VectorData.PointerFinish; (void)++first, ++current)
        {
            *current = *first;
        }
        if (first == last)
        {
            this->EraseAtEndInternal(current);
        }
        else
        {
            this->RangeInsertInternal(this->GetEnd(), first, last,
                                      std::iterator_traits<TInputIterator>::iterator_category());
        }
    }

    template <typename TForwardIterator>
    constexpr void AssignInternal(TForwardIterator first, TForwardIterator last, std::forward_iterator_tag)
    {
        const SizeTypeAlias currentSize = this->GetSize();
        const auto rangeSize = static_cast<SizeTypeAlias>(std::distance(first, last));

        if (rangeSize > this->GetCapacity())
        {
            CheckInitLengthInternal(rangeSize, this->GetTpAllocator());

            PointerAlias temp(this->AllocateAndCopy(rangeSize, first, last));
            Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);
            this->DeallocateInternal(this->VectorData.PointerStart,
                                     this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);
            this->VectorData.PointerStart = temp;
            this->VectorData.PointerFinish = temp + rangeSize;
            this->VectorData.PointerEndOfStorage = this->VectorData.PointerFinish;
        }
        else if (currentSize >= rangeSize)
        {
            this->EraseAtEndInternal(Utilities::Copy(first, last, this->VectorData.PointerStart));
        }
        else
        {
            TForwardIterator mid = first;
            std::advance(mid, currentSize);
            Utilities::Copy(first, mid, this->VectorData.PointerStart);
            [[maybe_unused]] const SizeTypeAlias addedSize = rangeSize - currentSize;
            this->VectorData.PointerFinish = Utilities::UninitializedCopy(mid, last, this->VectorData.PointerFinish);
        }
    }

    constexpr void FillAssignInternal(SizeTypeAlias size, ConstReferenceAlias value)
    {
        const SizeTypeAlias currentSize = this->GetSize();

        if (size > this->GetCapacity())
        {
            Vector temp(size, value, this->GetTpAllocator());
            temp.VectorData.SwapDataInternal(this->VectorData);
        }
        else if (size > currentSize)
        {
            Utilities::Fill(this->GetBegin(), this->GetEnd(), value);
            const SizeTypeAlias addedSize = size - currentSize;
            Utilities::UninitializedFillElements(this->VectorData.PointerFinish, addedSize, value);
        }
        else
        {
            this->EraseAtEndInternal(Utilities::FillElements(this->VectorData.PointerStart, size, value));
        }
    }

    constexpr void FillInsertInternal(ConstIteratorAlias position, SizeTypeAlias size, ConstReferenceAlias value)
    {
        if (size != 0)
        {
            if (SizeTypeAlias(this->VectorData.PointerEndOfStorage - this->VectorData.PointerFinish) >= size)
            {
                TemporaryValue temp(this, value);
                ValueTypeAlias &valueRef = temp.GetValueInternal();
                const SizeTypeAlias elementsAfter = this->GetEnd() - position;
                PointerAlias oldFinish(this->VectorData.PointerFinish);

                if (elementsAfter > size)
                {
                    Utilities::UninitializedMove(this->GetEnd() - size, this->GetEnd(), this->GetEnd());
                    this->VectorData.PointerFinish += size;
                    Utilities::Move(position.Base(), oldFinish - size, oldFinish);
                    Utilities::Fill(position.Base(), position.Base() + size, valueRef);
                }
                else
                {
                    this->VectorData.PointerFinish =
                        Utilities::UninitializedFillElements(oldFinish, size - elementsAfter, valueRef);
                    Utilities::UninitializedMove(position.Base(), oldFinish, this->VectorData.PointerFinish);
                    this->VectorData.PointerFinish += elementsAfter;
                    Utilities::Fill(position.Base(), oldFinish, valueRef);
                }
            }
            else
            {
                const SizeTypeAlias length = this->Available(size);
                const SizeTypeAlias elementsBefore = position - this->GetBegin();
                PointerAlias newStart(this->AllocateInternal(length));
                PointerAlias newFinish(newStart);
                try
                {
                    Utilities::UninitializedFillElements(newStart + elementsBefore, size, value);
                    newFinish = PointerAlias();
                    newFinish = Utilities::UninitializedMove(this->GetBegin(), position.Base(), newStart);
                    newFinish += size;
                    newFinish = Utilities::UninitializedMove(position.Base(), this->VectorData.PointerStart, newFinish);
                }
                catch (...)
                {
                    if (!newFinish)
                    {
                        Utilities::Destroy(newStart + elementsBefore, newStart + elementsBefore + size);
                    }
                    else
                    {
                        Utilities::Destroy(newStart, newFinish);
                    }
                    AllocatorTraitsAlias::deallocate(this->VectorData, newStart, length);
                    throw;
                }
                Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);
                this->DeallocateInternal(this->VectorData.PointerStart,
                                         this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);

                this->VectorData.PointerStart = newStart;
                this->VectorData.PointerFinish = newFinish;
                this->VectorData.PointerEndOfStorage = newStart + length;
            }
        }
    }

    template <typename TInteger>
    constexpr void InsertInternal(IteratorAlias position, TInteger size, TInteger value, std::true_type)
    {
        this->FillInsertInternal(position, static_cast<SizeTypeAlias>(size), value);
    }

    template <typename TInputIterator>
    constexpr void InsertInternal(IteratorAlias position, TInputIterator first, TInputIterator last, std::false_type)
    {
        using IteratorCategory = typename std::iterator_traits<TInputIterator>::iterator_category;
        this->RangeInsertInternal(position, first, last, IteratorCategory());
    }

    template <typename TInputIterator>
    constexpr void RangeInsertInternal(IteratorAlias position, TInputIterator first, TInputIterator last,
                                       std::input_iterator_tag)
    {
        if (position == this->GetEnd())
        {
            for (; first != last; ++first)
            {
                this->Insert(this->GetEnd(), *first);
            }
        }
        else if (first != last)
        {
            TemporaryValue temp(first, last, this->GetTpAllocator());
            this->Insert(position, std::make_move_iterator(temp.GetBegin()), std::make_move_iterator(temp.GetEnd()));
        }
    }

    template <typename TForwardIterator>
    constexpr void RangeInsertInternal(IteratorAlias position, TForwardIterator first, TForwardIterator last,
                                       std::forward_iterator_tag)
    {
        if (first != last)
        {
            const SizeTypeAlias size = std::distance(first, last);

            if (SizeTypeAlias(this->VectorData.PointerEndOfStorage - this->VectorData.PointerFinish) >= size)
            {
                const SizeTypeAlias elementsAfter = this->GetEnd() - position;
                PointerAlias oldFinish(this->VectorData.PointerFinish);

                if (elementsAfter > size)
                {
                    Utilities::UninitializedMove(this->VectorData.PointerFinish - size, this->VectorData.PointerFinish,
                                                 this->VectorData.PointerFinish);
                    this->VectorData.PointerFinish += size;
                    std::move(position.Base(), oldFinish - size, oldFinish);
                    std::copy(first, last, position);
                }
                else
                {
                    TForwardIterator mid = first;
                    std::advance(mid, elementsAfter);
                    Utilities::UninitializedCopy(mid, last, this->VectorData.PointerFinish);
                    this->VectorData.PointerFinish += size - elementsAfter;
                    Utilities::UninitializedMove(position.Base(), oldFinish, this->VectorData.PointerFinish);
                    this->VectorData.PointerFinish += elementsAfter;
                    Utilities::Copy(first, mid, position);
                }
            }
            else
            {
                PointerAlias oldStart = this->VectorData.PointerStart;
                PointerAlias oldFinish = this->VectorData.PointerFinish;
                const SizeTypeAlias length = this->Available(size);
                PointerAlias newStart(this->AllocateInternal(length));
                PointerAlias newFinish(newStart);
                try
                {
                    newFinish = Utilities::UninitializedMove(oldStart, position.Base(), newStart);
                    newFinish = Utilities::UninitializedCopy(first, last, newFinish);
                    newFinish = Utilities::UninitializedMove(position.Base(), oldFinish, newFinish);
                }
                catch (...)
                {
                    Utilities::Destroy(newStart, newFinish);
                    AllocatorTraitsAlias::deallocate(this->VectorData, newStart, length);
                }
                Utilities::Destroy(oldStart, oldFinish);
                this->DeallocateInternal(oldStart, this->VectorData.PointerEndOfStorage - oldStart);
                this->VectorData.PointerStart = newStart;
                this->VectorData.PointerFinish = newFinish;
                this->VectorData.PointerEndOfStorage = newStart + length;
            }
        }
    }

    template <typename TInputIterator>
    constexpr void InsertInternal(ConstIteratorAlias position, TInputIterator first, TInputIterator last,
                                  std::input_iterator_tag)
    {
        this->RangeInsertInternal(position, first, last, std::input_iterator_tag());
    }

    template <typename Arg> constexpr void DefaultInsertInternal(IteratorAlias position, Arg &&arg)
    {
        AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish,
                                        std::move(*(this->VectorData.PointerFinish - 1)));
        ++this->VectorData.PointerFinish;
        std::move_backward(position, this->GetEnd() - 2, this->GetEnd() - 1);
        *position = std::forward<Arg>(arg);
    }

    constexpr void DefaultAppendInternal(SizeTypeAlias size)
    {
        if (size != 0)
        {
            if (SizeTypeAlias(this->VectorData.PointerEndOfStorage - this->VectorData.PointerFinish) >= size)
            {
                this->VectorData.PointerFinish =
                    Utilities::UninitializedFillElements(this->VectorData.PointerFinish, size, ValueTypeAlias());
            }
            else
            {
                const SizeTypeAlias length = this->Available(size);
                const SizeTypeAlias oldSize = this->GetSize();
                PointerAlias newStart(this->AllocateInternal(length));
                PointerAlias destroyFrom = PointerAlias();

                try
                {
                    Utilities::UninitializedFillElements(newStart + oldSize, size, ValueTypeAlias());
                    destroyFrom = newStart + oldSize;
                    Utilities::UninitializedMove(this->VectorData.PointerStart, this->VectorData.PointerFinish,
                                                 newStart);
                }
                catch (...)
                {
                    if (destroyFrom)
                    {
                        Utilities::Destroy(destroyFrom, destroyFrom + size);
                    }
                    AllocatorTraitsAlias::deallocate(this->VectorData, newStart, length);
                    throw;
                }
                Utilities::Destroy(this->VectorData.PointerStart, this->VectorData.PointerFinish);
                this->DeallocateInternal(this->VectorData.PointerStart,
                                         this->VectorData.PointerEndOfStorage - this->VectorData.PointerStart);
                this->VectorData.PointerStart = newStart;
                this->VectorData.PointerFinish = newStart + oldSize + size;
                this->VectorData.PointerEndOfStorage = newStart + length;
            }
        }
    }

    constexpr bool ShrinkToFitInternal()
    {
        if (this->GetCapacity() == this->GetSize())
        {
            return false;
        }
        return Utilities::ShrinkToFitHelper<Vector>::DoIt(*this);
    }

    template <typename... Args> constexpr void ReallocateInsertInternal(IteratorAlias position, Args &&...args)
    {
        const SizeTypeAlias length = this->Available(1);

        PointerAlias oldStart = this->VectorData.PointerStart;
        PointerAlias oldFinish = this->VectorData.PointerFinish;
        const SizeTypeAlias elementsBefore = position - this->GetBegin();
        PointerAlias newStart(this->AllocateInternal(length));
        PointerAlias newFinish(newStart);

        struct Guard
        {
            PointerAlias Storage;
            SizeTypeAlias Length;
            TpAllocatorTypeAlias &Allocator;

            constexpr Guard(PointerAlias storage, SizeTypeAlias length, TpAllocatorTypeAlias &alloc) noexcept
                : Storage(storage), Length(length), Allocator(alloc)
            {
            }

            constexpr ~Guard() noexcept
            {
                if (Storage)
                {
                    AllocatorTraitsAlias::deallocate(Allocator, Storage, Length);
                }
            }

          public:
            Guard(const Guard &) = delete;
            Guard &operator=(const Guard &) = delete;
        };
        {
            Guard guard(newStart, length, this->GetTpAllocator());

            AllocatorTraitsAlias::construct(this->VectorData, std::to_address(newStart + elementsBefore),
                                            std::forward<Args>(args)...);
            struct GuardElms
            {
                PointerAlias First, Last;
                TpAllocatorTypeAlias &Allocator;

                constexpr GuardElms(PointerAlias elt, TpAllocatorTypeAlias &alloc) noexcept
                    : First(elt), Last(elt + 1), Allocator(alloc)
                {
                }

                constexpr ~GuardElms() noexcept
                {
                    Utilities::Destroy(First, Last);
                }

              public:
                GuardElms(const GuardElms &) = delete;
                GuardElms &operator=(const GuardElms &) = delete;
            };

            GuardElms guardElms(newStart + elementsBefore, this->GetTpAllocator());

            newFinish = Utilities::UninitializedMove(oldStart, position.Base(), newStart);
            ++newFinish;

            guardElms.First = newStart;
            newFinish = Utilities::UninitializedMove(position.Base(), oldFinish, newFinish);

            guardElms.First = oldStart;
            guardElms.Last = oldFinish;

            guard.Storage = oldStart;
            guard.Length = this->VectorData.PointerEndOfStorage - oldStart;
        }
        this->VectorData.PointerStart = newStart;
        this->VectorData.PointerFinish = newFinish;
        this->VectorData.PointerEndOfStorage = newStart + length;
    }

    template <typename... Args> constexpr void ReallocateAppendInternal(Args &&...args)
    {
        const SizeTypeAlias length = this->Available(1);
        PointerAlias oldStart = this->VectorData.PointerStart;
        PointerAlias oldFinish = this->VectorData.PointerFinish;
        PointerAlias newStart(this->AllocateInternal(length));
        PointerAlias newFinish(newStart);
        const SizeTypeAlias elements = this->GetEnd() - this->GetBegin();

        struct Guard
        {
            PointerAlias Storage;
            SizeTypeAlias Length;
            TpAllocatorTypeAlias &Allocator;

            constexpr Guard(PointerAlias storage, SizeTypeAlias length, TpAllocatorTypeAlias &alloc) noexcept
                : Storage(storage), Length(length), Allocator(alloc)
            {
            }

            constexpr ~Guard() noexcept
            {
                if (Storage)
                {
                    AllocatorTraitsAlias::deallocate(Allocator, Storage, Length);
                }
            }

            Guard(const Guard &) = delete;
        };
        {
            Guard guard(newStart, length, this->GetTpAllocator());

            AllocatorTraitsAlias::construct(this->VectorData, std::to_address(newStart + elements),
                                            std::forward<Args>(args)...);

            {
                struct GuardElms
                {
                    PointerAlias First, Last;
                    TpAllocatorTypeAlias &Allocator;

                    constexpr GuardElms(PointerAlias elt, TpAllocatorTypeAlias &alloc) noexcept
                        : First(elt), Last(elt + 1), Allocator(alloc)
                    {
                    }

                    constexpr ~GuardElms() noexcept
                    {
                        Utilities::Destroy(First, Last);
                    }

                    GuardElms(const GuardElms &) = delete;
                };

                GuardElms guardElms(newStart + elements, this->GetTpAllocator());

                newFinish = Utilities::UninitializedMove(oldStart, oldFinish, newStart);

                ++newFinish;
                guardElms.First = oldStart;
                guardElms.Last = oldFinish;
            }

            guard.Storage = oldStart;
            guard.Length = this->VectorData.PointerEndOfStorage - oldStart;
        }

        this->VectorData.PointerStart = newStart;
        this->VectorData.PointerFinish = newFinish;
        this->VectorData.PointerEndOfStorage = newStart + length;
    }

    constexpr IteratorAlias InsertRvalueInternal(ConstIteratorAlias position, ValueTypeAlias &&value)
    {
        const auto offset = position - this->GetConstBegin();

        if (this->VectorData.PointerFinish != this->VectorData.PointerEndOfStorage)
        {
            if (position == this->GetEnd())
            {
                AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish, std::move(value));
                ++this->VectorData.PointerFinish;
            }
            else
            {
                this->DefaultInsertInternal(this->GetBegin() + offset, std::move(value));
            }
        }
        else
        {
            this->ReallocateInsertInternal(this->GetBegin() + offset, std::move(value));
        }

        return IteratorAlias(this->GetBegin() + offset);
    }

    template <typename... Args>
    constexpr IteratorAlias DefaultEmplaceInternal(ConstIteratorAlias position, Args &&...args)
    {
        const auto offset = position - this->GetConstBegin();
        if (this->VectorData.PointerFinish != this->VectorData.PointerEndOfStorage)
        {
            if (position == this->GetConstEnd())
            {
                AllocatorTraitsAlias::construct(this->VectorData, this->VectorData.PointerFinish,
                                                std::forward<Args>(args)...);
                ++this->VectorData.PointerFinish;
            }
            else
            {
                TemporaryValue temp(this, std::forward<Args>(args)...);
                this->DefaultInsertInternal(this->GetBegin() + offset, std::move(temp.GetValueInternal()));
            }
        }
        else
        {
            this->ReallocateInsertInternal(this->GetBegin() + offset, std::forward<Args>(args)...);
        }

        return IteratorAlias(this->GetBegin() + offset);
    }

    constexpr IteratorAlias DefaultEmplaceInternal(ConstIteratorAlias position, ValueTypeAlias &&value)
    {
        return this->InsertRvalueInternal(position, std::move(value));
    }

    [[nodiscard]] constexpr SizeTypeAlias Available(SizeTypeAlias size) const noexcept
    {
        if (GetMaxSize() - GetSize() < size)
        {
            throw VectorOutOfBoundsException(__FILE__, __LINE__);
        }

        const size_t length = GetSize() + (std::max)(GetSize(), size);

        return (length < GetSize() || length > GetMaxSize()) ? GetMaxSize() : length;
    }

    constexpr void EraseAtEndInternal(PointerAlias position)
    {
        if (SizeTypeAlias diff = this->VectorData.PointerFinish - position)
        {
            Utilities::Destroy(position, this->VectorData.PointerFinish);
            this->VectorData.PointerFinish = position;
        }
    }

    constexpr IteratorAlias EraseInternal(IteratorAlias position)
    {
        if (position + 1 != this->GetEnd())
        {
            Utilities::Move(position + 1, this->GetEnd(), position);
        }

        --this->VectorData.PointerFinish;
        AllocatorTraitsAlias::destroy(this->VectorData, this->VectorData.PointerFinish);
        return position;
    }

    constexpr IteratorAlias EraseInternal(IteratorAlias first, IteratorAlias last)
    {
        if (first != last)
        {
            if (last != this->GetEnd())
            {
                Utilities::Move(last, this->GetEnd(), first);
            }
            this->EraseAtEndInternal(last.Base() + (this->GetEnd() - last));
        }
        return first;
    }

  protected:
    template <typename TUp> constexpr TUp *GetDataPointerInternal(TUp *p) const noexcept
    {
        return p;
    }

    template <typename TUp>
    typename std::pointer_traits<TUp>::element_type *GetDataPointerInternal(TUp *p) const noexcept
    {
        return this->IsEmpty() ? nullptr : std::addressof(*p);
    }

    static constexpr SizeTypeAlias CheckInitLengthInternal(SizeTypeAlias length, SizeTypeAlias max_size)
    {
        if (length > max_size)
        {
            throw VectorOutOfBoundsException(__FILE__, __LINE__);
        }
        return length;
    }

    static constexpr SizeTypeAlias CheckMaxSize(const TpAllocatorTypeAlias &alloc)
    {
        const SizeTypeAlias maxSize = AllocatorTraitsAlias::max_size(alloc);
        const SizeTypeAlias diffMax = std::numeric_limits<ptrdiff_t>::max() / sizeof(T);

        return (std::min)(diffMax, maxSize);
    }

    template <typename TForwardIterator>
    constexpr PointerAlias AllocateAndCopy(SizeTypeAlias size, TForwardIterator first, TForwardIterator last)
    {
        PointerAlias result = this->AllocateInternal(size);
        try
        {
            Utilities::UninitializedCopy(first, last, result);
            return result;
        }
        catch (...)
        {
            this->DeallocateInternal(result, size);
            throw;
        }
    }

  private:
    // Zone of DSA
    template <typename TCompare>
    constexpr void MergeHelper(SizeTypeAlias leftStart, SizeTypeAlias mid, SizeTypeAlias rightEnd, TCompare comp)
    {
        SizeTypeAlias leftSize = mid - leftStart + 1;
        SizeTypeAlias rightSize = rightEnd - mid;

        Vector<ValueTypeAlias> left(leftSize);
        Vector<ValueTypeAlias> right(rightSize);

        for (SizeTypeAlias i = 0; i < leftSize; ++i)
        {
            left[i] = (*this)[leftStart + i];
        }

        for (SizeTypeAlias i = 0; i < rightSize; ++i)
        {
            right[i] = (*this)[mid + 1 + i];
        }

        SizeTypeAlias leftIndex = 0;
        SizeTypeAlias rightIndex = 0;
        SizeTypeAlias mergedIndex = leftStart;

        while (leftIndex < leftSize && rightIndex < rightSize)
        {
            if (comp(left[leftIndex], right[rightIndex]))
            {
                (*this)[mergedIndex] = left[leftIndex];
                ++leftIndex;
            }
            else
            {
                (*this)[mergedIndex] = right[rightIndex];
                ++rightIndex;
            }
            ++mergedIndex;
        }

        while (leftIndex < leftSize)
        {
            (*this)[mergedIndex] = left[leftIndex];
            ++leftIndex;
            ++mergedIndex;
        }

        while (rightIndex < rightSize)
        {
            (*this)[mergedIndex] = right[rightIndex];
            ++rightIndex;
            ++mergedIndex;
        }
    }

    template <typename TCompare> constexpr void QuickSortHelper(IteratorAlias begin, IteratorAlias end, TCompare comp)
    {
        if (begin < end)
        {
            IteratorAlias pivot = this->PartitionForQuickSortHelper(begin, end, comp);
            this->QuickSortHelper(begin, pivot - 1, comp);
            this->QuickSortHelper(pivot + 1, end, comp);
        }
    }

    template <typename TCompare>
    constexpr IteratorAlias PartitionForQuickSortHelper(IteratorAlias begin, IteratorAlias end, TCompare comp)
    {
        ValueTypeAlias pivot = *end;
        IteratorAlias i = begin - 1;
        for (IteratorAlias j = begin; j < end; ++j)
        {
            if (comp(*j, pivot))
            {
                ++i;
                std::swap(*i, *j);
            }
        }
        std::swap(*(i + 1), *end);
        return i + 1;
    }

    template <typename TCompare> constexpr void HeapifyHelper(SizeTypeAlias size, SizeTypeAlias index, TCompare comp)
    {
        SizeTypeAlias largest = index;
        SizeTypeAlias left = 2 * index + 1;
        SizeTypeAlias right = 2 * index + 2;

        if (left < size && !comp((*this)[left], (*this)[largest]))
        {
            largest = left;
        }

        if (right < size && !comp((*this)[right], (*this)[largest]))
        {
            largest = right;
        }

        if (largest != index)
        {
            std::swap((*this)[index], (*this)[largest]);
            this->HeapifyHelper(size, largest, comp);
        }
    }

  public:
    template <typename TCompare> constexpr void BubbleSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        for (IteratorAlias i = this->GetBegin(); i != this->GetEnd(); ++i)
        {
            for (IteratorAlias j = this->GetBegin(); j != this->GetEnd() - 1; ++j)
            {
                if (comp(*(j + 1), *j))
                {
                    std::swap(*j, *(j + 1));
                }
            }
        }
    }

    template <typename TCompare> constexpr void SelectionSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        for (IteratorAlias i = this->GetBegin(); i != this->GetEnd(); ++i)
        {
            IteratorAlias min = i;
            for (IteratorAlias j = i + 1; j != this->GetEnd(); ++j)
            {
                if (comp(*j, *min))
                {
                    min = j;
                }
            }
            std::swap(*i, *min);
        }
    }

    template <typename TCompare> constexpr void InsertionSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        for (IteratorAlias i = this->GetBegin() + 1; i != this->GetEnd(); ++i)
        {
            ValueTypeAlias key = std::move(*i);
            IteratorAlias j = i - 1;
            while (j >= this->GetBegin() && comp(key, *j))
            {
                *(j + 1) = std::move(*j);
                --j;
            }
            *(j + 1) = std::move(key);
        }
    }

    template <typename TCompare> constexpr void MergeSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        // iterative merge sort

        for (SizeTypeAlias currSize = 1; currSize < this->GetSize(); currSize *= 2)
        {
            for (SizeTypeAlias leftStart = 0; leftStart < this->GetSize() - 1; leftStart += 2 * currSize)
            {
                SizeTypeAlias mid = (std::min)(leftStart + currSize - 1, this->GetSize() - 1);
                SizeTypeAlias rightEnd = (std::min)(leftStart + 2 * currSize - 1, this->GetSize() - 1);

                this->MergeHelper(leftStart, mid, rightEnd, comp);
            }
        }
    }

    template <typename TCompare> constexpr void QuickSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        this->QuickSortHelper(this->GetBegin(), this->GetEnd() - 1, comp);
    }

    template <typename TCompare> constexpr void HeapSort(TCompare comp)
    {
        if (this->GetSize() < 2)
        {
            return;
        }

        for (SizeTypeAlias i = this->GetSize() / 2 - 1; true; --i)
        {
            this->HeapifyHelper(this->GetSize(), i, comp);
            if (i == 0)
            {
                break;
            }
        }

        for (SizeTypeAlias i = this->GetSize() - 1; i > 0; --i)
        {
            std::swap((*this)[0], (*this)[i]);
            this->HeapifyHelper(i, 0, comp);
        }
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
            this->MergeSort(compare);
            break;
        }
    }
};

template <typename T, typename Alloc>
constexpr inline bool operator==(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.GetBegin(), lhs.GetEnd(), rhs.GetBegin()));
}

template <typename T, typename Alloc>
constexpr inline bool operator!=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
constexpr inline bool operator<(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.GetBegin(), lhs.GetEnd(), rhs.GetBegin(), rhs.GetEnd());
}

template <typename T, typename Alloc>
constexpr inline bool operator<=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
constexpr inline bool operator>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
constexpr inline bool operator>=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs)
{
    return !(lhs < rhs);
}

} // namespace DSALibraries::Containers

#endif