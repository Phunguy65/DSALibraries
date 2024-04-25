//
// Created by PNguyen on 21/02/2024.
//

#ifndef STACKTEST_H
#define STACKTEST_H
#include "../googletest/include/gtest/gtest.h"
#include "../include/Containers//Stack/Stack.hpp"

using namespace DSALibraries::Containers;
namespace DSALibraries
{
namespace Test
{
class StackTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        for (int i = 0; i < 5; i++)
        {
            Stack_0.Push(i);
        }

        for (int i = 0; i < 10000; i++)
        {
            Stack_1.Push(i);
        }
    }

    void TearDown() override
    {
        Stack_0.Clear();
        Stack_1.Clear();
    }

    DSALibraries::Stack<int, 5> Stack_0;
    DSALibraries::Stack<int, 10000> Stack_1;
};

TEST_F(StackTest, Push)
{
    ASSERT_EQ(Stack_0.GetSize(), 5);
    ASSERT_EQ(Stack_1.GetSize(), 10000);
}
} // namespace Test
} // namespace DSALibraries

#endif // STACKTEST_H
