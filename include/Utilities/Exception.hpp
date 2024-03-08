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
    typedef std::string string_type;
    typedef int numeric_type;

    Exception(string_type message, string_type fileText, numeric_type line) noexcept
        : _message(std::move(message)), _fileText(std::move(fileText)), _lineNumber(line)
    {
    }

    [[nodiscard]] constexpr string_type GetMessage() const
    {
        return _message;
    }

    [[nodiscard]] constexpr string_type GetFile() const
    {
        return _fileText;
    }

    [[nodiscard]] constexpr numeric_type GetLineNumber() const
    {
        return _lineNumber;
    }

    [[nodiscard]] constexpr string_type GetFullMessage() const
    {
        return GetMessage() + "with file " + GetFile() + " at line " + std::to_string(GetLineNumber());
    }

    [[nodiscard]] constexpr string_type GetMessageWithLine() const
    {
        return GetMessage() + " at line " + std::to_string(GetLineNumber());
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        if (_fileText.empty())
        {
            auto message = GetMessageWithLine();
            auto returnMessage = new char[message.size() + 1];
            std::strcpy(returnMessage, message.c_str());
            return returnMessage;
        }
        else
        {
            auto message = GetFullMessage();
            auto returnMessage = new char[message.size() + 1];
            std::strcpy(returnMessage, message.c_str());
            return returnMessage;
        }
    }

  private:
    string_type _message;
    string_type _fileText;
    numeric_type _lineNumber;
};
} // namespace Utilities
} // namespace DSALibraries

#endif // LIBRARIES_EXCEPTION_H
