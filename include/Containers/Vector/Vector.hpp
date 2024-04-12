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
#include "VectorBase.hpp"
#include <cstddef>
namespace DSALibraries::Containers
{

template <typename T> class Vector : public DSALibraries::Containers::IVector<T>
{
  public:
    using ValueTypeAlias = typename DSALibraries::Containers::IVector<T>::ValueTypeAlias;
    using ReferenceTypeAlias = typename DSALibraries::Containers::IVector<T>::ReferenceTypeAlias;
    using ConstReferenceTypeAlias = typename DSALibraries::Containers::IVector<T>::ConstReferenceTypeAlias;
    using PointerTypeAlias = typename DSALibraries::Containers::IVector<T>::PointerTypeAlias;
    using ConstPointerTypeAlias = typename DSALibraries::Containers::IVector<T>::ConstPointerTypeAlias;
    using IteratorAlias = typename DSALibraries::Containers::IVector<T>::IteratorAlias;
    using ConstIteratorAlias = typename DSALibraries::Containers::IVector<T>::ConstIteratorAlias;
    using ReverseIteratorAlias = typename DSALibraries::Containers::IVector<T>::ReverseIteratorAlias;
    using ConstReverseIteratorAlias = typename DSALibraries::Containers::IVector<T>::ConstReverseIteratorAlias;
    using SizeTypeAlias = typename DSALibraries::Containers::IVector<T>::SizeTypeAlias;

    // Adaptive alias
    using value_type = ValueTypeAlias;
    using reference = ReferenceTypeAlias;
    using const_reference = ConstReferenceTypeAlias;
    using pointer = PointerTypeAlias;
    using const_pointer = ConstPointerTypeAlias;
    using iterator = IteratorAlias;
    using const_iterator = ConstIteratorAlias;
    using reverse_iterator = ReverseIteratorAlias;
    using const_reverse_iterator = ConstReverseIteratorAlias;
    using size_type = SizeTypeAlias;

    Vector(void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
    }

    explicit Vector(SizeTypeAlias initialSize, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        this->Initialize();
        this->Resize(initialSize);
    }

    Vector(SizeTypeAlias initialSize, ConstReferenceTypeAlias value, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        this->Initialize();
        this->Resize(initialSize, value);
    }

    template <typename TIterator>
    Vector(TIterator first, TIterator last, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        this->Assign(first, last);
    }

    Vector(std::initializer_list<ValueTypeAlias> initList, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        this->Assign(initList.begin(), initList.end());
    }

    Vector(const Vector &rhs, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        this->Assign(rhs.GetConstBegin(), rhs.GetConstEnd());
    }

    Vector(Vector &&rhs, void *pBuffer, SizeTypeAlias capacity)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(pBuffer), capacity)
    {
        if (this != &rhs)
        {
            this->Initialize();

            IteratorAlias first = rhs.GetBegin();

            while (first != rhs.GetEnd())
            {
                this->PushBack(std::move(*first));
                ++first;
            }

            rhs.Initialize();
        }
    }

    Vector &operator=(const Vector &rhs)
    {
        if (this != &rhs)
        {
            this->Assign(rhs.GetConstBegin(), rhs.GetConstEnd());
        }

        return *this;
    }

    Vector &operator=(Vector &&rhs)
    {
        if (this != &rhs)
        {
            this->Clear();

            IteratorAlias first = rhs.GetBegin();

            while (first != rhs.GetEnd())
            {
                this->PushBack(std::move(*first));
                ++first;
            }

            rhs.Initialize();
        }

        return *this;
    }

    virtual void Repair() override
    {
        return;
    }

    ~Vector()
    {
        this->Clear();
    }
};

template <typename T, const size_t MAX_SIZE> class FVector : public DSALibraries::Containers::IVector<T>
{

  public:
    using ValueTypeAlias = typename DSALibraries::Containers::IVector<T>::ValueTypeAlias;
    using ReferenceTypeAlias = typename DSALibraries::Containers::IVector<T>::ReferenceTypeAlias;
    using ConstReferenceTypeAlias = typename DSALibraries::Containers::IVector<T>::ConstReferenceTypeAlias;
    using PointerTypeAlias = typename DSALibraries::Containers::IVector<T>::PointerTypeAlias;
    using ConstPointerTypeAlias = typename DSALibraries::Containers::IVector<T>::ConstPointerTypeAlias;
    using IteratorAlias = typename DSALibraries::Containers::IVector<T>::IteratorAlias;
    using ConstIteratorAlias = typename DSALibraries::Containers::IVector<T>::ConstIteratorAlias;
    using ReverseIteratorAlias = typename DSALibraries::Containers::IVector<T>::ReverseIteratorAlias;
    using ConstReverseIteratorAlias = typename DSALibraries::Containers::IVector<T>::ConstReverseIteratorAlias;
    using SizeTypeAlias = typename DSALibraries::Containers::IVector<T>::SizeTypeAlias;
    using DifferenceTypeAlias = typename DSALibraries::Containers::IVector<T>::DifferenceTypeAlias;

    // Adaptive alias
    using value_type = ValueTypeAlias;
    using reference = ReferenceTypeAlias;
    using const_reference = ConstReferenceTypeAlias;
    using pointer = PointerTypeAlias;
    using const_pointer = ConstPointerTypeAlias;
    using iterator = IteratorAlias;
    using const_iterator = ConstIteratorAlias;
    using reverse_iterator = ReverseIteratorAlias;
    using const_reverse_iterator = ConstReverseIteratorAlias;
    using size_type = SizeTypeAlias;
    using difference_type = DifferenceTypeAlias;

  public:
    static const size_t MAX_SIZE_VALUE = MAX_SIZE;

    FVector() : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Initialize();
    }

    explicit FVector(size_t initialSize)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Initialize();
        this->Resize(initialSize);
    }

    FVector(size_t initialSize, const T &value)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Initialize();
        this->Resize(initialSize, value);
    }

    template <typename TIterator>
    FVector(TIterator first, TIterator last)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Assign(first, last);
    }

    FVector(std::initializer_list<T> initList)
        : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Assign(initList.begin(), initList.end());
    }

    FVector(const FVector &rhs) : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        this->Assign(rhs.GetConstBegin(), rhs.GetConstEnd());
    }

    FVector(FVector &&rhs) : DSALibraries::Containers::IVector<T>(reinterpret_cast<T *>(_buffer), MAX_SIZE)
    {
        if (this != &rhs)
        {
            this->Initialize();

            IteratorAlias first = rhs.GetBegin();

            while (first != rhs.GetEnd())
            {
                this->PushBack(std::move(*first));
                ++first;
            }

            rhs.Initialize();
        }
    }

    FVector &operator=(const FVector &rhs)
    {
        if (this != &rhs)
        {
            this->Assign(rhs.GetConstBegin(), rhs.GetConstEnd());
        }

        return *this;
    }

    FVector &operator=(FVector &&rhs)
    {
        if (this != &rhs)
        {
            this->Clear();

            IteratorAlias first = rhs.GetBegin();

            while (first != rhs.GetEnd())
            {
                this->PushBack(std::move(*first));
                ++first;
            }

            rhs.Initialize();
        }

        return *this;
    }

    virtual void Repair() override
    {
        this->RepairBuffer(reinterpret_cast<T *>(_buffer));
    }

    ~FVector()
    {
        this->Clear();
    }

  private:
    alignas(T) std::byte _buffer[MAX_SIZE * sizeof(T)];
};

template <typename... T> FVector(T...) -> FVector<typename std::common_type_t<T...>, sizeof...(T)>;

} // namespace DSALibraries::Containers
#endif