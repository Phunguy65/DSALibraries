#ifndef DSA_LIBRARIES_DLIST_TEST_HPP
#define DSA_LIBRARIES_DLIST_TEST_HPP

#include "../../include/Containers/DList/DList.hpp"
#include "DList/DListIterator.hpp"
#include <gtest/gtest.h>
namespace DSALibraries::Test
{
class DListTest : public ::testing::Test
{
  protected:
    Containers::DList<int> DList_0;
    void SetUp() override
    {
        auto size = 10;
        for (int i = 0; i < size; i++)
        {
            DList_0.PushFront(i);
        }
    }

    void TearDown() override
    {
        DList_0.Clear();
    }
};

TEST_F(DListTest, insert_after_between_position)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        if (*it == 5)
        {
            DList_0.InsertAfter(it, -1);
            break;
        }
    }
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;

        if (*it == -1)
        {
            ASSERT_EQ(*it, -1);
        }
    }
}

TEST_F(DListTest, remove_if)
{
    DList_0.RemoveIf([](int x) { return x == 4; });
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
}

TEST_F(DListTest, insert_before_between_position)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        if (*it == 5)
        {
            DList_0.InsertBefore(it, -1);
            break;
        }
    }
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;

        if (*it == -1)
        {
            ASSERT_EQ(*it, -1);
        }
    }
}

TEST_F(DListTest, remove)
{

    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.Remove(5);
    ASSERT_EQ(DList_0.GetSize(), 9);
}

TEST_F(DListTest, insert_after_end)
{
    Containers::DListIterator<int> it = DList_0.GetBegin();
    while (it.PointerNext() != DList_0.GetEnd())
    {
        it++;
    }
    DList_0.InsertAfter(it, 11);
    ASSERT_EQ(DList_0.GetSize(), 11);
}

TEST_F(DListTest, insert_before_begin)
{
    Containers::DListIterator<int> it = DList_0.GetBegin();
    DList_0.InsertBefore(it, -1);
    ASSERT_EQ(DList_0.GetSize(), 11);
}

TEST_F(DListTest, reverse)
{
    DList_0.Reverse();
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    ASSERT_EQ(DList_0.GetSize(), 10);
}

TEST_F(DListTest, insertion_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(1, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(DListTest, selection_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(2, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(DListTest, bubble_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(3, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(DListTest, quick_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(4, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(DListTest, heap_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(5, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(DListTest, merge_sort)
{
    for (auto it = DList_0.GetBegin(); it != DList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    DList_0.DSASort(6, std::less<>());
    auto it = DList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}
} // namespace DSALibraries::Test
#endif