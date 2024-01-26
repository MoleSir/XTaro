#pragma once

#include <string>
#include <array>
#include <cstdarg>

namespace xtaro
{

    class Console
    {
    public:
        Console() = default;
        ~Console() noexcept = default;
        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console& operator = (const Console&) = delete;
        Console& operator = (Console&&) = delete;

    public:
        void debug(const std::string& message) const;
        void info(const std::string& message) const;
        void warning(const std::string& message) const;
        void error(const std::string& message) const;
        
    private:
        void print(const char* level, const std::string& message) const;

        void printMessage(const std::string& message) const;
        void printLevel(const char* level) const;
    };

}