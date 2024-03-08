//
// Created by PNguyen on 20/02/2024.
//

#ifndef DSA_LIBRARIES_STACK_H
#define DSA_LIBRARIES_STACK_H

#include "../../Utilities/Exception.hpp"
#include "StackExceptions.hpp"
#include <algorithm>
#include <type_traits>
#include <utility>

using namespace DSALibraries::Utilities;
namespace DSALibraries
{
namespace Containers
{

class StackBase
{
  public:
    typedef size_t SizeTypeAlias;

    [[nodiscard]] bool IsEmpty() const
    {
        return TopIndex == 0;
    }

    [[nodiscard]] bool IsFull() const
    {
        return CurrentSize == Capacity;
    }

    SizeTypeAlias GetSize() const
    {
        return CurrentSize;
    }

    SizeTypeAlias GetMaxSize() const
    {
        return Capacity;
    }

    SizeTypeAlias GetAvailableSpace() const
    {
        return GetMaxSize() - GetSize();
    }

  protected:
    explicit StackBase(SizeTypeAlias capacity) : CurrentSize(0), TopIndex(0), Capacity(capacity)
    {
    }

    ~StackBase() = default;

    void AddIn()
    {
        TopIndex = CurrentSize++;
    }

    void DeleteOut()
    {
        --CurrentSize;
        --TopIndex;
    }

    void ClearIndex()
    {
        CurrentSize = 0;
        TopIndex = 0;
    }

    SizeTypeAlias CurrentSize;
    SizeTypeAlias TopIndex;
    const SizeTypeAlias Capacity;
};

template <typename T> class IterStack : public Containers::StackBase
{
  public:
    typedef T ValueTypeAlias;
    typedef T& ReferenceTypeAlias;
    typedef const T& ConstReferenceTypeAlias;
    typedef T* PointerTypeAlias;
    typedef const T* ConstPointerTypeAlias;
    typedef StackBase::SizeTypeAlias SizeTypeAlias;

  private:
    typedef typename Containers::StackBase _stackBaseAlias;

    T* _bufferInternal;

  public:
    IterStack(const IterStack&) = delete;

    ReferenceTypeAlias Top()
    {
        return _bufferInternal[TopIndex];
    }

    ConstReferenceTypeAlias Top() const
    {
        return _bufferInternal[TopIndex];
    }

    void Push(ConstReferenceTypeAlias value)
    {
        if (IsFull())
        {
            throw StackFullException(__FILE__, __LINE__);
        }
        _stackBaseAlias::AddIn();
        ::new (&_bufferInternal[TopIndex]) T(value);
    }

    void Push(ValueTypeAlias&& value)
    {
        if (IsFull())
        {
            throw StackFullException("", 7);
        }
        _stackBaseAlias::AddIn();
        ::new (&_bufferInternal[TopIndex]) T(std::move(value));
    }

    template <typename... Args> void Emplace(Args&&... args)
    {
        if (IsFull())
        {
            throw StackFullException(__FILE__, __LINE__);
        }
        _stackBaseAlias::AddIn();
        ::new (&_bufferInternal[TopIndex]) T(std::forward<Args>(args)...);
    }

    void Emplace()
    {
        if (IsFull())
        {
            throw StackFullException(__FILE__, __LINE__);
        }
        _stackBaseAlias::AddIn();
        ::new (&_bufferInternal[TopIndex]) T();
    }

    void Clear()
    {
        if (std::is_trivially_destructible<T>::value)
        {
            _stackBaseAlias::ClearIndex();
        }
        else
        {
            while (CurrentSize > 0)
            {
                _bufferInternal[TopIndex].~T();
                _stackBaseAlias::DeleteOut();
            }
        }
    }

    void Pop()
    {
        if (IsEmpty())
        {
            throw StackEmptyException(__FILE__, __LINE__);
        }
        _bufferInternal[TopIndex].~T();
        _stackBaseAlias::DeleteOut();
    }

    template <typename TContainer> void PopInto(TContainer& container)
    {
        if (IsEmpty())
        {
            throw StackEmptyException(__FILE__, __LINE__);
        }
        container.Push(std::move(_bufferInternal[TopIndex]));
        Pop();
    }

    void PopInto(T&& value)
    {
        if (IsEmpty())
        {
            throw StackEmptyException(__FILE__, __LINE__);
        }
        value = std::move(Top());
        Pop();
    }

    void Reverse()
    {
        if (IsEmpty())
        {
            throw StackEmptyException(__FILE__, __LINE__);
        }
        std::reverse(_bufferInternal, _bufferInternal + CurrentSize);
    }

    IterStack& operator=(const IterStack& other)
    {
        if (this != &other)
        {
            Clear();
            Clone(other);
        }
        return *this;
    }

    IterStack& operator=(IterStack&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            Clone(std::move(other));
        }
        return *this;
    }

  protected:
    void Clone(const IterStack& other)
    {
        Clear();

        SizeTypeAlias index = 0;

        for (SizeTypeAlias i = 0; i < other.GetSize(); ++i)
        {
            Push(other._bufferInternal[index++]);
        }
    }

    void Clone(IterStack&& other)
    {
        Clear();

        SizeTypeAlias index = 0;

        for (SizeTypeAlias i = 0; i < other.GetSize(); ++i)
        {
            Push(std::move(other._bufferInternal[index++]));
        }
    }

    IterStack(T* buffer, SizeTypeAlias capacity)
        : _stackBaseAlias(capacity), _bufferInternal(buffer){

                                     };

  public:
    ~IterStack() = default;
};
} // namespace Containers

template <typename T, const size_t SIZE> class Stack : public Containers::IterStack<T>
{
  private:
    alignas(T) std::byte _buffer[sizeof(T) * SIZE];

  public:
    Stack() : Containers::IterStack<T>(reinterpret_cast<T*>(&_buffer[0]), SIZE)
    {
    }

    Stack(const Stack& rhs) : Containers::IterStack<T>(reinterpret_cast<T*>(&_buffer[0]), SIZE)
    {
        Containers::IterStack<T>::Clone(rhs);
    }

    Stack(Stack&& rhs) noexcept : Containers::IterStack<T>(reinterpret_cast<T*>(&_buffer[0]), SIZE)
    {
        Containers::IterStack<T>::Clone(std::move(rhs));
    }

    ~Stack()
    {
        Containers::IterStack<T>::Clear();
    }

    Stack& operator=(const Stack& rhs)
    {
        if (this != &rhs)
        {
            Containers::IterStack<T>::Clone(rhs);
        }
        return *this;
    }

    Stack& operator=(Stack&& rhs) noexcept
    {
        if (this != &rhs)
        {
            Containers::IterStack<T>::Clone(std::move(rhs));
        }
        return *this;
    }
};
} // namespace DSALibraries

#endif // LIBRARIES_STACK_H
