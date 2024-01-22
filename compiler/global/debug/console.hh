#pragma once

#include <string>
#include <array>
#include <cstdarg>

namespace xtaro
{
    enum class ConsoleLevel
    {
        DEBUG, 
        INFO, 
        WARNING, 
        ERROR, 
        FATAL, 
        SIZE,
    };
    #define CONSOLE_LEVEL_SIZE\
    static_cast<unsigned>(ConsoleLevel::SIZE)

    enum class ConsoleColor
    {
        BLACK,
        DEEP_RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        DEEP_GREEN,
        WHITE,
        SIZE,
    };
    #define CONSOLE_COLOR_SIZE\
    static_cast<unsigned>(ConsoleColor::SIZE)

    class Console
    {
    public:
        static Console* instance();

    public:
        void setColor(ConsoleColor color);

        void print(const char* fmt, ...) const;
        void print(const std::string& message) const;

    public:
        void debug(const char* fmt, ...) const;
        void debug(const std::string& message) const;

        void info(const char* fmt, ...) const;
        void info(const std::string& message) const;

        void warning(const char* fmt, ...) const;
        void warning(const std::string& message) const;

        void error(const char* fmt, ...) const;
        void error(const std::string& message) const;

        void fatal(const char* fmt, ...) const;
        void fatal(const std::string& message) const;

    private:
        void echo(ConsoleLevel level, const char* fmt, va_list args) const;
        void echo(ConsoleLevel level, const std::string& message) const;

        void echoMessage(const std::string& message) const;
        void echoLevel(ConsoleLevel level) const;
        std::string generateColorMessage(const char* fmt, va_list args) const;

    private:
        Console() = default;
        ~Console() noexcept = default;
        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console& operator = (const Console&) = delete;
        Console& operator = (Console&&) = delete;

    private:
        static std::array<int, CONSOLE_COLOR_SIZE> fontColors;
        static std::array<const char*, CONSOLE_LEVEL_SIZE> levelStrings;

    private:
        ConsoleColor _color{ConsoleColor::WHITE};
        ConsoleLevel _level{ConsoleLevel::DEBUG};
    };

    extern Console* console;
}