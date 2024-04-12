#ifndef DSA_LIBRARIES_VECTOR_TEST_HPP
#define DSA_LIBRARIES_VECTOR_TEST_HPP
#include "../../include/Containers/Vector/Vector.hpp"
#include "../googletest/include/gtest/gtest.h"
namespace DSALibraries::Test
{
class VectorTest : public ::testing::Test
{
  protected:
    DSALibraries::Containers::FVector<int, 20> Vector_0;
    void SetUp() override
    {
        auto size = 10;
        for (int i = 0; i < size; i++)
        {
            Vector_0.PushBack(i);
        }
    }

    void TearDown() override
    {
        Vector_0.Clear();
    }
};

TEST_F(VectorTest, PushBack)
{
    ASSERT_EQ(Vector_0.GetSize(), 10);
}

TEST_F(VectorTest, PopBack)
{
    Vector_0.PopBack();
    ASSERT_EQ(Vector_0.GetSize(), 9);
}

TEST_F(VectorTest, EmplaceBack)
{
    Vector_0.EmplaceBack(10);
    ASSERT_EQ(Vector_0.GetSize(), 11);
}

TEST_F(VectorTest, Clear)
{
    Vector_0.Clear();
    ASSERT_EQ(Vector_0.GetSize(), 0);
    ASSERT_EQ(Vector_0.Capacity(), 20);
}

TEST_F(VectorTest, Resize)
{
    Vector_0.Resize(20);
    ASSERT_EQ(Vector_0.GetSize(), 20);
    ASSERT_EQ(Vector_0.Capacity(), 20);
}

TEST_F(VectorTest, Empty)
{
    ASSERT_FALSE(Vector_0.IsEmpty());
    Vector_0.Clear();
    ASSERT_TRUE(Vector_0.IsEmpty());
}

TEST_F(VectorTest, At)
{
    ASSERT_EQ(Vector_0.At(0), 0);
    ASSERT_EQ(Vector_0.At(1), 1);
    ASSERT_EQ(Vector_0.At(2), 2);
    ASSERT_EQ(Vector_0.At(3), 3);
    ASSERT_EQ(Vector_0.At(4), 4);
    ASSERT_EQ(Vector_0.At(5), 5);
    ASSERT_EQ(Vector_0.At(6), 6);
    ASSERT_EQ(Vector_0.At(7), 7);
    ASSERT_EQ(Vector_0.At(8), 8);
    ASSERT_EQ(Vector_0.At(9), 9);
}

TEST_F(VectorTest, OperatorSquareBracket)
{
    ASSERT_EQ(Vector_0[0], 0);
    ASSERT_EQ(Vector_0[1], 1);
    ASSERT_EQ(Vector_0[2], 2);
    ASSERT_EQ(Vector_0[3], 3);
    ASSERT_EQ(Vector_0[4], 4);
    ASSERT_EQ(Vector_0[5], 5);
    ASSERT_EQ(Vector_0[6], 6);
    ASSERT_EQ(Vector_0[7], 7);
    ASSERT_EQ(Vector_0[8], 8);
    ASSERT_EQ(Vector_0[9], 9);
}

TEST_F(VectorTest, Insert)
{
    Vector_0.Insert(Vector_0.GetBegin(), 100);
    ASSERT_EQ(Vector_0.GetSize(), 11);
    ASSERT_EQ(Vector_0[0], 100);
    ASSERT_EQ(Vector_0[1], 0);
    ASSERT_EQ(Vector_0[2], 1);
    ASSERT_EQ(Vector_0[3], 2);
    ASSERT_EQ(Vector_0[4], 3);
    ASSERT_EQ(Vector_0[5], 4);
    ASSERT_EQ(Vector_0[6], 5);
    ASSERT_EQ(Vector_0[7], 6);
    ASSERT_EQ(Vector_0[8], 7);
    ASSERT_EQ(Vector_0[9], 8);
    ASSERT_EQ(Vector_0[10], 9);
}

TEST_F(VectorTest, Erase)
{
    Vector_0.Erase(Vector_0.GetBegin());
    ASSERT_EQ(Vector_0.GetSize(), 9);
    ASSERT_EQ(Vector_0[0], 1);
    ASSERT_EQ(Vector_0[1], 2);
    ASSERT_EQ(Vector_0[2], 3);
    ASSERT_EQ(Vector_0[3], 4);
    ASSERT_EQ(Vector_0[4], 5);
    ASSERT_EQ(Vector_0[5], 6);
    ASSERT_EQ(Vector_0[6], 7);
    ASSERT_EQ(Vector_0[7], 8);
    ASSERT_EQ(Vector_0[8], 9);
}

TEST_F(VectorTest, EmplaceBackWhenFull)
{
    Vector_0.Resize(20);
    ASSERT_ANY_THROW(Vector_0.EmplaceBack(10));
}

TEST_F(VectorTest, AtWhenIndexOutOfRange)
{
    ASSERT_ANY_THROW(Vector_0.At(100));
}

TEST_F(VectorTest, OperatorSquareBracketWhenIndexOutOfRange)
{
    ASSERT_ANY_THROW(Vector_0[100]);
}

TEST_F(VectorTest, Sort)
{
    std::sort(Vector_0.GetBegin(), Vector_0.GetEnd(), std::greater<int>());
    ASSERT_EQ(Vector_0[0], 9);
}
} // namespace DSALibraries::Test
#endif