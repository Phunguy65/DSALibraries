#ifndef DSA_LIBRARIES_SLIST_TEST_HPP
#define DSA_LIBRARIES_SLIST_TEST_HPP
#include "../googletest/include/gtest/gtest.h"
#include "../include/Containers/SList/SList.hpp"
#include <string>

namespace DSALibraries::Containers
{

class Person
{
  public:
    int Age;
    std::string Name;
    Person(int age, std::string name) : Age(age), Name(name)
    {
    }

    std::string toString()
    {
        return "Name: " + Name + " Age: " + std::to_string(Age);
    }
};

class SListTest : public ::testing::Test
{

  protected:
    SList<Person> SList_0;
    void SetUp() override
    {
        for (auto i = 0; i < 10; i++)
        {
            SList_0.PushFront(Person(i, "Name"));
        }
    }

    void TearDown() override
    {
        SList_0.Clear();
    }
};

TEST_F(SListTest, PRINT)
{
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << it->toString() << std::endl;
    }

    auto itInsert = SList_0.GetBegin();
    while (itInsert.PointerNext() != SList_0.GetEnd())
    {
        itInsert++;
    }
    SList_0.InsertAfter(itInsert, Person(100, "Inserted"));
    for (auto it = SList_0.GetBegin(); it != SList_0.GetEnd(); it++)
    {
        std::cout << it->toString() << std::endl;
    }

    SList_0.RemoveIf([](Person p) { return p.Age == 100; });
}
} // namespace DSALibraries::Containers
#endif