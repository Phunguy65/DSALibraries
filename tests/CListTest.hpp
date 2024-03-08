#include <cstddef>
#include <iostream>
#ifndef DSA_LIBRARIES_CLIST_TEST_HPP
#define DSA_LIBRARIES_CLIST_TEST_HPP 1
#include "../../include/Containers/CList/CList.hpp"
#include <gtest/gtest.h>
namespace DSALibraries::Containers
{
class CListTest : public ::testing::Test
{
  protected:
    CList<int> list;
    void SetUp() override
    {
        auto size = 10;
        list.EmplaceFront(0);
        auto it = list.GetBegin();
        for (auto i = 1; i < size; i++)
        {
            list.InsertAfter(it, i);
            ++it;
        }
    }
    void TearDown() override
    {
        list.Clear();
    }
};
TEST_F(CListTest, EmplaceFront)
{
    list.EmplaceFront(10);
    ASSERT_EQ(list.GetItemFront(), 10);
}
TEST_F(CListTest, Traverse)
{
    auto it = list.GetBegin();
    for (auto i = 0; i < 10; i++)
    {
        ASSERT_EQ(*it, i);
        ++it;
    }
    auto data = *it;
    ASSERT_EQ(data, 1);
}
TEST_F(CListTest, Clear)
{
    list.Clear();
    ASSERT_EQ(list.IsEmpty(), true);
}
TEST_F(CListTest, InsertAfter)
{
    auto it = list.GetBegin();
    while (*it != 5)
    {
        ++it;
    }
    list.InsertAfter(it, -1);

    for (auto it = list.GetBegin(); it != list.GetEnd(); ++it)
    {
        std::cout << *it << std::endl;
        if (*it == -1)
        {
            ASSERT_EQ(*it, -1);
        }
    }

    ASSERT_EQ(list.GetSize(), 11);
}

TEST_F(CListTest, RemoveAfter)
{
    list.RemoveIf([](int i) { return i == 5; });
    auto it = list.GetBegin();
    while (it != list.GetEnd())
    {
        std::cout << *it << std::endl;
        ++it;
    }
    ASSERT_TRUE(*it != 0);
    ASSERT_EQ(list.GetSize(), 9);
    ++it;
    ASSERT_EQ(*it, list.GetItemFront());
}
} // namespace DSALibraries::Containers
#endif