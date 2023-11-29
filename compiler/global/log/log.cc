#include "log.hh"

#include <ctime>
#include <cstring>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdarg>

#include "../allocator/allocator.hh"

namespace xtaro
{
    Logger* logger = Logger::instance();

    const char* Logger::_levelStrings[Level::COUNT] = {
        "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };

    Logger* Logger::_instance = nullptr;

    char Logger::_buffer[Logger::MAX_BUFFER_SIZE] = {0};


    Logger::Logger() noexcept:
        _fileOut{}, _level{Logger::Level::DEBUG} {}

    Logger::~Logger() noexcept {}
 
    Logger* Logger::instance() noexcept
    {
        if (Logger::_instance == nullptr)
        {
            try
            {
                Logger::_instance = new Logger();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Create logger object failed: " << e.what() << '\n';
                exit(11);
            }
        }
            
        return Logger::_instance;
    }

    void Logger::open(const std::string& fileName) noexcept
    {
        try
        {
            this->_fileOut.open(fileName);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Open log file failed: " << e.what() << '\n';
            exit(12);
        }
    }

    void Logger::close() noexcept
    {
        try
        {
            this->_fileOut.close();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Close log file failed: " << e.what() << '\n';
            exit(12);
        }
    }

    void Logger::log(Logger::Level level, const char* format, va_list args)
    {
        // Check level
        if (this->_level > level) 
            return;

        // Current time to string
        time_t current_ticks = time(NULL);
        struct tm* time = localtime(&current_ticks);

        char timeStr[32] = {0};
        memset(timeStr, 0, sizeof(timeStr));
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", time);

        // Format data time and file stringF
        const char* formatContext = "[%s] %s > ";
        int size = snprintf(Logger::_buffer, 128, formatContext, timeStr, Logger::_levelStrings[level]);
        if (size < 0)
        {
            this->_fileOut << "Log failed.\n";
            return;
        }
        Logger::_buffer[size] = 0;
        this->_fileOut << Logger::_buffer;

        // Format mutable params: infomation
        size = vsnprintf(Logger::_buffer, 128, format, args);
        if (size < 0)
        {
            this->_fileOut << "Log failed.\n";
            return;
        }
        Logger::_buffer[size] = 0;
        this->_fileOut << Logger::_buffer;

        // Flush file
        this->_fileOut << '\n';
        this->_fileOut.flush();
    }

    void Logger::debug(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::DEBUG, format, args);
        va_end(args);
    }

    void Logger::info(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::INFO, format, args);
        va_end(args);
    }

    void Logger::warning(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::WARNING, format, args);
        va_end(args);
    }

    void Logger::error(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::ERROR, format, args);
        va_end(args);
    }

    void Logger::fatal(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::FATAL, format, args);
        va_end(args);
    }
}
