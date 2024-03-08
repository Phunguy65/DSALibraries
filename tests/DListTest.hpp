#ifndef DSA_LIBRARIES_DLIST_TEST_HPP
#define DSA_LIBRARIES_DLIST_TEST_HPP

#include "../../include/Containers/DList/DList.hpp"
#include "DList/DListIterator.hpp"
#include <gtest/gtest.h>
namespace DSALibraries::Containers
{
class DListTest : public ::testing::Test
{
  protected:
    DList<int> DList_0;
    void SetUp() override
    {
        auto size = 10;
        auto it = DList_0.GetBeginFromHead();
        for (int i = 0; i < size; i++)
        {
            DList_0.InsertAfter(it, i);
            it++;
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
    DListIterator<int> it = DList_0.GetBegin();
    while (it.PointerNext() != DList_0.GetEnd())
    {
        it++;
    }
    DList_0.InsertAfter(it, 11);
    ASSERT_EQ(DList_0.GetSize(), 11);
}

TEST_F(DListTest, insert_before_begin)
{
    DListIterator<int> it = DList_0.GetBegin();
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

} // namespace DSALibraries::Containers
#endif