#ifndef DSA_LIBRARIES_SLIST_TEST_HPP
#define DSA_LIBRARIES_SLIST_TEST_HPP
#include "../googletest/include/gtest/gtest.h"
#include "../include/Containers/SList/SList.hpp"
#include <exception>

namespace DSALibraries::Containers
{
class SListTest : public ::testing::Test
{

  protected:
    SList<int> SList_0;
    void SetUp() override
    {
        for (auto i = 0; i < 10; i++)
        {
            SList_0.PushFront(i);
        }
    }

    void TearDown() override
    {
        SList_0.Clear();
    }
};

TEST_F(SListTest, ItemFront)
{
    ASSERT_EQ(SList_0.GetItemFront(), 0);
}

TEST_F(SListTest, Iterator)
{
    for (int i = 0; i < 20; i++)
    {
        SList_0.PushFront(i);
    }
    SList<int>::IteratorAlias iter = SList_0.GetBegin();

    for (int i = 19; i >= 0; i--)
    {
        ASSERT_EQ(*iter, i);
        iter++;
    }
}

TEST_F(SListTest, Sort)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); ++it)
    {
        std::cout << *it << std::endl;
    }

    SList_0.Sort();
    auto it_throw_null = SList_0.GetBegin();

    while (it_throw_null != SList_0.GetEnd())
    {
        std::cout << *it_throw_null << std::endl;
        ++it_throw_null;
    }

    std:: << *it_throw_null << std::endl;
}

} // namespace DSALibraries::Containers
#endif