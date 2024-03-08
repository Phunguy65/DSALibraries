#ifndef DSA_LIBRARIES_LIST_EXCEPTIONS_HPP
#define DSA_LIBRARIES_LIST_EXCEPTIONS_HPP
#include "../../Utilities/Exception.hpp"
namespace DSALibraries::Containers
{
class ListException : public Utilities::Exception
{
  public:
    ListException(string_type message, string_type fileText, int line)
        : Exception(std::move(message), std::move(fileText), line)
    {
    }
};

class ListEmptyException : public ListException
{
  public:
    ListEmptyException(string_type fileText, numeric_type line)
        : ListException("List is empty", std::move(fileText), line)
    {
    }
};

class ListFullException : public ListException
{
  public:
    ListFullException(string_type fileText, numeric_type line)
        : ListException("List is full", std::move(fileText), line)
    {
    }
};

class ListIteratorException : public ListException
{
  public:
    ListIteratorException(string_type fileText, int line) : ListException("list::iterator", std::move(fileText), line)
    {
    }
};
} // namespace DSALibraries::Containers
#endif