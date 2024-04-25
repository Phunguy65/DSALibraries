//
// Created by PNguyen on 21/02/2024.
//

#ifndef DSA_LIBRARIES_LIST_TEST_H
#define DSA_LIBRARIES_LIST_TEST_H
#include "../googletest/include/gtest/gtest.h"
#include "../include/Containers/List/List.hpp"

namespace DSALibraries::Test
{
class ListTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        for (int i = 0; i < 5; i++)
        {
            List_0.PushBack(i);
        }

        for (int i = 0; i < 10000; i++)
        {
            List_1.PushBack(i);
        }
    }

    void TearDown() override
    {
        List_0.Clear();
        List_1.Clear();
    }

    List<int> List_0;
    List<int> List_1;
};

TEST_F(ListTest, PushBack)
{
    ASSERT_EQ(List_0.GetSize(), 5);
    ASSERT_EQ(List_1.GetSize(), 10000);
}

TEST_F(ListTest, PopBack)
{
    List_0.PopBack();
    List_1.PopBack();
    ASSERT_EQ(List_0.GetSize(), 4);
    ASSERT_EQ(List_1.GetSize(), 9999);
}

TEST_F(ListTest, PushFront)
{
    List_0.PushFront(10);
    List_1.PushFront(10);
    ASSERT_EQ(List_0.GetSize(), 6);
    ASSERT_EQ(List_1.GetSize(), 10001);
}

TEST_F(ListTest, PopFront)
{
    List_0.PopFront();
    List_1.PopFront();
    ASSERT_EQ(List_0.GetSize(), 4);
    ASSERT_EQ(List_1.GetSize(), 9999);
}

TEST_F(ListTest, Clear)
{
    List_0.Clear();
    List_1.Clear();
    ASSERT_EQ(List_0.GetSize(), 0);
    ASSERT_EQ(List_1.GetSize(), 0);
}

TEST_F(ListTest, GetSize)
{
    ASSERT_EQ(List_0.GetSize(), 5);
    ASSERT_EQ(List_1.GetSize(), 10000);
}

TEST_F(ListTest, IsEmpty)
{
    ASSERT_EQ(List_0.IsEmpty(), false);
    ASSERT_EQ(List_1.IsEmpty(), false);
}

TEST_F(ListTest, GetFront)
{
    ASSERT_EQ(List_0.GetItemFront(), 0);
    ASSERT_EQ(List_1.GetItemFront(), 0);
}

TEST_F(ListTest, GetBack)
{
    ASSERT_EQ(List_0.GetItemBack(), 4);
    ASSERT_EQ(List_1.GetItemBack(), 9999);
}

TEST_F(ListTest, EmplaceBack)
{
    List_0.EmplaceBack(10);
    List_1.EmplaceBack(10);
    ASSERT_EQ(List_0.GetItemBack(), 10);
    ASSERT_EQ(List_1.GetItemBack(), 10);
}

}; // namespace DSALibraries::Test
#endif