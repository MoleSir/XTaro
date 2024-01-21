#include "logger.hh"

#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdarg>

#include <allocator/allocator.hh>

namespace xtaro
{
    Logger* logger = Logger::instance();

    const char* Logger::_levelStrings[Level::COUNT] = {
        "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };

    char Logger::_buffer[Logger::MAX_BUFFER_SIZE] = {0};

    Logger::Logger():
        _fileOut{}, _level{Logger::Level::DEBUG} {}
         
    Logger* Logger::instance() noexcept
    {
        static Logger _logger;
        return &_logger;
    }

    void Logger::open(const std::string& fileName)
    {
        this->_fileOut.open(fileName);
    }

    void Logger::close()
    {
        this->_fileOut.close();
    }

    void Logger::log(Logger::Level level, const char* format, va_list args)
    {
        // Check level
        if (this->_level > level) 
            return;

        // Current time to string
        time_t current_ticks = std::time(NULL);
        struct tm* time = std::localtime(&current_ticks);

        char timeStr[32] = {0};
        std::memset(timeStr, 0, sizeof(timeStr));
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", time);

        // Format data time and file stringF
        const char* formatContext = "[%s] %s > ";
        int size = std::snprintf(Logger::_buffer, 128, formatContext, timeStr, Logger::_levelStrings[level]);
        if (size < 0)
        {
            this->_fileOut << "Log failed.\n";
            return;
        }
        Logger::_buffer[size] = '\0';
        this->_fileOut << Logger::_buffer;

        // Format mutable params: infomation
        size = std::vsnprintf(Logger::_buffer, 128, format, args);
        if (size < 0)
        {
            this->_fileOut << "Log failed.\n";
            return;
        }
        Logger::_buffer[size] = '\0';
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

    void Logger::debug(const std::string& message)
    {
        this->debug(message.c_str());
    }

    void Logger::info(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::INFO, format, args);
        va_end(args);
    }

    void Logger::info(const std::string& message)
    {
        this->info(message.c_str());
    }

    void Logger::warning(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::WARNING, format, args);
        va_end(args);
    }

    void Logger::warning(const std::string& message)
    {
        this->warning(message.c_str());
    }

    void Logger::error(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::ERROR, format, args);
        va_end(args);
    }

    void Logger::error(const std::string& message)
    {
        this->error(message.c_str());
    }

    void Logger::fatal(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->log(Logger::Level::FATAL, format, args);
        va_end(args);
    }

    void Logger::fatal(const std::string& message)
    {
        this->fatal(message.c_str());
    }

}
