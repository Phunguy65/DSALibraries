#ifndef DSA_LIBRARIES_SLIST_TEST_HPP
#define DSA_LIBRARIES_SLIST_TEST_HPP
#include "../googletest/include/gtest/gtest.h"
#include "../include/Containers/SList/SList.hpp"
#include <string>

namespace DSALibraries::Test
{
class SListTest : public ::testing::Test
{
  protected:
    Containers::SList<int> SList_0;
    void SetUp() override
    {
        auto size = 10;
        for (int i = 0; i < size; i++)
        {
            SList_0.PushFront(i);
        }
    }

    void TearDown() override
    {
        SList_0.Clear();
    }
};

TEST_F(SListTest, PushFront)
{
    SList_0.PushFront(10);
    ASSERT_EQ(SList_0.GetItemFront(), 10);
}

TEST_F(SListTest, insertion_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(1, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(SListTest, selection_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(2, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(SListTest, bubble_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(3, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(SListTest, quick_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(4, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(SListTest, heap_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(5, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}

TEST_F(SListTest, merge_sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << *it << std::endl;
    }
    SList_0.DSASort(6, std::less<>());
    auto it = SList_0.GetBegin();
    for (int i = 0; i < 10; i++)
    {
        std::cout << *it << std::endl;
        it++;
    }
}
} // namespace DSALibraries::Test

#endif