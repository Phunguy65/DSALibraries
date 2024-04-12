#ifndef TEST_DEFAULTS_CPP
#define TEST_DEFAULTS_CPP
#include "../googletest/include/gtest/gtest.h"
#include "VectorTest.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif