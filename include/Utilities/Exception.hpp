//
// Created by PNguyen on 20/02/2024.
//

#ifndef DSA_UTILITIES_EXCEPTION_HPP
#define DSA_UTILITIES_EXCEPTION_HPP

#include <cstring>
#include <exception>
#include <string>
#include <utility>

namespace DSALibraries
{
namespace Utilities
{
class Exception : std::exception
{
  public:
    typedef const char *string_type;
    typedef int numeric_type;

    Exception(string_type message, string_type fileText, numeric_type line) noexcept
        : _message(std::move(message)), _fileName(std::move(fileText)), _lineNumber(line)
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return _message;
    }

    [[nodiscard]] string_type GetFileName() const noexcept
    {
        return _fileName;
    }

    [[nodiscard]] numeric_type GetLineNumber() const noexcept
    {
        return _lineNumber;
    }

  private:
    string_type _message;
    string_type _fileName;
    numeric_type _lineNumber;
};
} // namespace Utilities
} // namespace DSALibraries

#endif // LIBRARIES_EXCEPTION_H
