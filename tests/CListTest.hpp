//
// Created by PNguyen on 19/04/2024.
//

#ifndef DSA_LIBRARIES_CLIST_TEST_H
#define DSA_LIBRARIES_CLIST_TEST_H
#include "../include/Containers/CList/CList.hpp"
#include <gtest/gtest.h>
namespace DSALibraries::Test
{
class CListTest : public ::testing::Test
{
  protected:
    Containers::CList<int> CList_0;
    void SetUp() override
    {
        auto size = 10;
        for (int i = 0; i < size; i++)
        {
            CList_0.PushFront(i);
        }
    }
    void TearDown() override
    {
        CList_0.Clear();
    }
};

TEST_F(CListTest, insert_after_between_position)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        if (*it == 5)
        {
            CList_0.InsertAfter(it, -1);
            break;
        }
    }
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
        if (*it == -1)
        {
            ASSERT_EQ(*it, -1);
        }
    }
}

TEST_F(CListTest, remove_if)
{
    CList_0.RemoveIf([](int x) { return x == 4; });
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
}

TEST_F(CListTest, insert_before_between_position)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        if (*it.PointerNext() == 5)
        {
            CList_0.InsertAfter(it, -1);
            break;
        }
    }
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
        if (*it == -1)
        {
            ASSERT_EQ(*it, -1);
        }
    }
}

TEST_F(CListTest, insertion_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(1, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(CListTest, selection_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(2, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(CListTest, bubble_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(3, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(CListTest, quick_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(4, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(CListTest, heap_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(5, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(CListTest, merge_sort)
{
    for (auto it = CList_0.GetBegin(); it != CList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    CList_0.DSASort(6, std::less<>());
    auto it = CList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}
} // namespace DSALibraries::Test
#endif // DSA_LIBRARIES_CLIST_TEST_H
