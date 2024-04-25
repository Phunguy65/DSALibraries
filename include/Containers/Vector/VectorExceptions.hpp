#ifndef DSA_LIBRARIES_VECTOR_EXCEPTIONS_HPP
#define DSA_LIBRARIES_VECTOR_EXCEPTIONS_HPP
#include "../../Utilities/Exception.hpp"
namespace DSALibraries::Containers
{
class VectorException : public Utilities::Exception
{
  public:
    VectorException(string_type message, string_type fileText, int line)
        : Exception(std::move(message), std::move(fileText), line)
    {
    }
};
class VectorFullException : public VectorException
{
  public:
    VectorFullException(string_type fileText, numeric_type line)
        : VectorException("Vector is full", std::move(fileText), line)
    {
    }
};
class VectorEmptyException : public VectorException
{
  public:
    VectorEmptyException(string_type fileText, numeric_type line)
        : VectorException("Vector is empty", std::move(fileText), line)
    {
    }
};
class VectorOutOfBoundsException : public VectorException
{
  public:
    VectorOutOfBoundsException(string_type fileText, numeric_type line)
        : VectorException("Index out of bounds", std::move(fileText), line)
    {
    }
};
class VectorInCompatibleTypeException : public VectorException
{
  public:
    VectorInCompatibleTypeException(string_type fileText, numeric_type line)
        : VectorException("Incompatible type", std::move(fileText), line)
    {
    }
};
} // namespace DSALibraries::Containers
#endif