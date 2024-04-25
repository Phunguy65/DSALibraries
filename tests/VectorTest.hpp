#ifndef DSA_LIBRARIES_VECTOR_TEST_HPP
#define DSA_LIBRARIES_VECTOR_TEST_HPP
#include "../../include/Containers/Vector/Vector.hpp"
#include "../googletest/include/gtest/gtest.h"
#include <vector>
namespace DSALibraries::Test
{
class VectorTest : public ::testing::Test
{
  protected:
    DSALibraries::Containers::Vector<int> VectorImpl;
    std::vector<int> VectorCompare;
    void SetUp() override
    {
        auto size = 10;
        for (int i = 0; i < size; i++)
        {
            VectorImpl.PushBack(i);
            VectorCompare.push_back(i);
        }
    }

    void TearDown() override
    {
        VectorImpl.Clear();
    }
};

TEST_F(VectorTest, PushBack)
{
    VectorImpl.PushBack(10);
    VectorCompare.push_back(10);
    std::cout << "VectorImpl: " << VectorImpl.GetSize() << std::endl;
    std::cout << "VectorCompare: " << VectorCompare.size() << std::endl;
    ASSERT_EQ(VectorImpl.GetSize(), VectorCompare.size());
}

TEST_F(VectorTest, EmplaceBack)
{
    ASSERT_EQ(VectorImpl.GetSize(), 10);
    std::cout << "VectorImpl: " << VectorImpl.GetSize() << std::endl;
}

TEST_F(VectorTest, Clear)
{
    VectorImpl.Clear();
    VectorCompare.clear();
    std::cout << "VectorImpl: " << VectorImpl.GetSize() << std::endl;
    std::cout << "VectorCompare: " << VectorCompare.size() << std::endl;
    std::cout << "VectorImpl: " << VectorImpl.GetCapacity() << std::endl;
    std::cout << "VectorCompare: " << VectorCompare.capacity() << std::endl;
    ASSERT_EQ(VectorImpl.GetSize(), VectorCompare.size());
    ASSERT_EQ(VectorImpl.GetCapacity(), VectorCompare.capacity());
}

TEST_F(VectorTest, Resize)
{
    VectorImpl.Resize(20);
    ASSERT_EQ(VectorImpl.GetSize(), 20);
    ASSERT_EQ(VectorImpl.GetCapacity(), 20);
}

TEST_F(VectorTest, Empty)
{
    ASSERT_FALSE(VectorImpl.IsEmpty());
    VectorImpl.Clear();
    ASSERT_TRUE(VectorImpl.IsEmpty());
}

TEST_F(VectorTest, At)
{
    ASSERT_EQ(VectorImpl.At(0), 0);
    ASSERT_EQ(VectorImpl.At(1), 1);
    ASSERT_EQ(VectorImpl.At(2), 2);
    ASSERT_EQ(VectorImpl.At(3), 3);
    ASSERT_EQ(VectorImpl.At(4), 4);
    ASSERT_EQ(VectorImpl.At(5), 5);
    ASSERT_EQ(VectorImpl.At(6), 6);
    ASSERT_EQ(VectorImpl.At(7), 7);
    ASSERT_EQ(VectorImpl.At(8), 8);
    ASSERT_EQ(VectorImpl.At(9), 9);
}

TEST_F(VectorTest, OperatorSquareBracket)
{
    ASSERT_EQ(VectorImpl[0], 0);
    ASSERT_EQ(VectorImpl[1], 1);
    ASSERT_EQ(VectorImpl[2], 2);
    ASSERT_EQ(VectorImpl[3], 3);
    ASSERT_EQ(VectorImpl[4], 4);
    ASSERT_EQ(VectorImpl[5], 5);
    ASSERT_EQ(VectorImpl[6], 6);
    ASSERT_EQ(VectorImpl[7], 7);
    ASSERT_EQ(VectorImpl[8], 8);
    ASSERT_EQ(VectorImpl[9], 9);
}

TEST_F(VectorTest, Insert)
{
    VectorImpl.Insert(VectorImpl.GetBegin(), 100);
    ASSERT_EQ(VectorImpl.GetSize(), 11);
    ASSERT_EQ(VectorImpl[0], 100);
    ASSERT_EQ(VectorImpl[1], 0);
    ASSERT_EQ(VectorImpl[2], 1);
    ASSERT_EQ(VectorImpl[3], 2);
    ASSERT_EQ(VectorImpl[4], 3);
    ASSERT_EQ(VectorImpl[5], 4);
    ASSERT_EQ(VectorImpl[6], 5);
    ASSERT_EQ(VectorImpl[7], 6);
    ASSERT_EQ(VectorImpl[8], 7);
    ASSERT_EQ(VectorImpl[9], 8);
    ASSERT_EQ(VectorImpl[10], 9);
}

TEST_F(VectorTest, Erase)
{
    VectorImpl.Erase(VectorImpl.GetBegin());
    ASSERT_EQ(VectorImpl.GetSize(), 9);
    ASSERT_EQ(VectorImpl[0], 1);
    ASSERT_EQ(VectorImpl[1], 2);
    ASSERT_EQ(VectorImpl[2], 3);
    ASSERT_EQ(VectorImpl[3], 4);
    ASSERT_EQ(VectorImpl[4], 5);
    ASSERT_EQ(VectorImpl[5], 6);
    ASSERT_EQ(VectorImpl[6], 7);
    ASSERT_EQ(VectorImpl[7], 8);
    ASSERT_EQ(VectorImpl[8], 9);
}

TEST_F(VectorTest, AtWhenIndexOutOfRange)
{
    ASSERT_ANY_THROW(VectorImpl.At(100));
}

TEST_F(VectorTest, OperatorSquareBracketWhenIndexOutOfRange)
{
    ASSERT_ANY_THROW(VectorImpl.At(100));
}

TEST_F(VectorTest, insertion_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.InsertionSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << *(++VectorImpl.GetEnd()) << std::endl;
}

TEST_F(VectorTest, merge_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.MergeSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << *(++VectorImpl.GetEnd()) << std::endl;
}

TEST_F(VectorTest, quick_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.QuickSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << *(++VectorImpl.GetEnd()) << std::endl;
}

TEST_F(VectorTest, bubble_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.BubbleSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << *(++VectorImpl.GetEnd()) << std::endl;
}

TEST_F(VectorTest, selection_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.SelectionSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::cout << *(++VectorImpl.GetEnd()) << std::endl;
}

TEST_F(VectorTest, heap_sort)
{
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    VectorImpl.HeapSort(std::greater<>());
    for (auto i = VectorImpl.GetBegin(); i != VectorImpl.GetEnd(); i++)
    {
        std::cout << *i << " ";
    }
}

} // namespace DSALibraries::Test
#endif