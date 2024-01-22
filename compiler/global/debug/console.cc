#include "console.hh"
#include <util/format.hh>
#include <iostream>

namespace xtaro
{
    #define INT(l) static_cast<int>(l)

    Console* console {Console::instance()};

    /*
        BLACK,
        DEEP_RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        DEEP_GREEN,
        WHITE,
    */
    std::array<int, CONSOLE_COLOR_SIZE> 
    Console::fontColors {
        30, 31, 32, 33, 34, 35, 36, 37,
    };

    /*
        DEBUG, 
        INFO, 
        WARNING, 
        ERROR, 
        FATAL, 
    */
    std::array<const char*, CONSOLE_LEVEL_SIZE> 
    Console::levelStrings {
        "\e[34mDEBUG\e[0m",
        "\e[32mINFO\e[0m",
        "\e[33mWARNIG\e[0m",
        "\e[31mERROR\e[0m",
        "\e[31mFATAL\e[0m",
    };

    void Console::setColor(ConsoleColor color)
    {
        this->_color = color;
    }

    void Console::print(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->print(util::format(fmt, args));
        va_end(args);
    }
    
    void Console::print(const std::string& message) const
    {
        std::cout << 
            util::format(
                "\e[%dm%s\e[0m",
                Console::fontColors[INT(this->_color)],
                message.c_str()
            ) 
                 << std::endl;
    }

    void Console::debug(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->echo(ConsoleLevel::DEBUG, fmt, args);
        va_end(args);
    }

    void Console::debug(const std::string& message) const
    {
        this->echo(ConsoleLevel::DEBUG, message);
    }

    void Console::info(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->echo(ConsoleLevel::INFO, fmt, args);
        va_end(args);
    }

    void Console::info(const std::string& message) const
    {
        this->echo(ConsoleLevel::INFO, message);
    }

    void Console::warning(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->echo(ConsoleLevel::WARNING, fmt, args);
        va_end(args);
    }

    void Console::warning(const std::string& message) const
    {
        this->echo(ConsoleLevel::WARNING, message);
    }

    void Console::error(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->echo(ConsoleLevel::ERROR, fmt, args);
        va_end(args);
    }

    void Console::error(const std::string& message) const
    {
        this->echo(ConsoleLevel::ERROR, message);
    }

    void Console::fatal(const char* fmt, ...) const
    {
        va_list args;
        va_start(args, fmt);
        this->echo(ConsoleLevel::FATAL, fmt, args);
        va_end(args);
    }

    void Console::fatal(const std::string& message) const
    {
        this->echo(ConsoleLevel::FATAL, message);
    }

    void Console::echo(ConsoleLevel level, const char* fmt, va_list args) const
    {
        this->echoLevel(level);
        this->echoMessage(util::format(fmt, args));
    }
    
    void Console::echo(ConsoleLevel level, const std::string& message) const
    {
        this->echoLevel(level);
        this->echoMessage(message);
    }

    void Console::echoMessage(const std::string& message) const
    {
        std::cout << message << std::endl;
    }

    void Console::echoLevel(ConsoleLevel level) const
    {
        std::cout << '[' << Console::levelStrings[INT(level)] << "]: ";
    }

    Console* Console::instance()
    {
        static Console _console;
        return &_console;
    }
}