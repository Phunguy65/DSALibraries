#ifndef DSA_LIBRARIES_STACK_EXCEPTIONS_HPP
#define DSA_LIBRARIES_STACK_EXCEPTIONS_HPP
#include "../../Utilities/Exception.hpp"
namespace DSALibraries::Containers
{
class StackException : public Utilities::Exception
{
  public:
    StackException(string_type message, string_type fileText, int line)
        : Exception(std::move(message), std::move(fileText), line)
    {
    }
};

class StackFullException : public StackException
{
  public:
    StackFullException(string_type fileText, numeric_type line)
        : StackException("Stack is full", std::move(fileText), line)
    {
    }
};

class StackEmptyException : public StackException
{
  public:
    StackEmptyException(string_type fileText, numeric_type line)
        : StackException("Stack is empty", std::move(fileText), line)
    {
    }
};
} // namespace DSALibraries::Containers

#endif