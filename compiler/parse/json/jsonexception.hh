#pragma once

#include "util/util.hh"
#include <exception>
#include <string>

namespace xtaro::parse
{
    class JsonScanException : public std::exception
    {
    public:
        JsonScanException(const char* message) noexcept:
            std::exception(),
            _message{message} {}

        virtual const char* what() const noexcept
        { return util::format("Scan json file excetopn: %s", this->_message.c_str()); }

    private:
        std::string _message;
    };

    class JsonParseException : public std::exception
    {
    public:
        JsonParseException(const char* message) noexcept:
            std::exception(),
            _message{message} {}

        virtual const char* what() const noexcept
        { return util::format("Parse json file excetopn: %s", this->_message.c_str()); }

    private:
        std::string _message;
    };
}