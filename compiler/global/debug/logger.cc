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
#include <util/format.hh>

namespace xtaro
{
    #define INT(l) static_cast<int>(l)

    Logger* logger = Logger::instance();

    std::array<const char*, LOGGER_LEVEL_SIZE> Logger::levelStrings {
        "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };

    void Logger::setLevel(LoggerLevel level)
    {
        this->_level = level;
    }
         
    Logger* Logger::instance()
    {
        static Logger _logger;
        return &_logger;
    }

    void Logger::open(const std::string& fileName)
    {
        this->_outfile.open(fileName);
    }

    void Logger::close()
    {
        this->_outfile.close();
    }

    void Logger::log(LoggerLevel level, const char* fmt, va_list args)
    {
        this->log(level, util::vformat(fmt, args));
    }

    void Logger::log(LoggerLevel level, const std::string& message)
    {
        // Check level
        if (this->_level > level) 
            return;

        this->logDateTime();
        this->logLevel(level);

        // Log message
        this->_outfile << message << '\n';

        // Flush file
        this->_outfile.flush();
    }

    void Logger::logDateTime()
    {
        time_t current_ticks = std::time(NULL);
        struct tm* time = std::localtime(&current_ticks);

        char dateTime[32] = {0};
        std::memset(dateTime, 0, sizeof(dateTime));
        std::strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", time);
        this->_outfile << '[' << dateTime << ']';
    }
    
    void Logger::logLevel(LoggerLevel level)
    {
        this->_outfile << ' ' << Logger::levelStrings[INT(level)] << " > ";
    }

    void Logger::debug(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->log(LoggerLevel::DEBUG, fmt, args);
        va_end(args);
    }

    void Logger::debug(const std::string& message)
    {
        this->log(LoggerLevel::DEBUG, message);
    }

    void Logger::info(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->log(LoggerLevel::INFO, fmt, args);
        va_end(args);
    }

    void Logger::info(const std::string& message)
    {
        this->log(LoggerLevel::INFO, message);
    }

    void Logger::warning(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->log(LoggerLevel::WARNING, fmt, args);
        va_end(args);
    }

    void Logger::warning(const std::string& message)
    {
        this->log(LoggerLevel::WARNING, message);
    }

    void Logger::error(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->log(LoggerLevel::ERROR, fmt, args);
        va_end(args);
    }

    void Logger::error(const std::string& message)
    {
        this->log(LoggerLevel::ERROR, message);
    }

    void Logger::fatal(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        this->log(LoggerLevel::FATAL, fmt, args);
        va_end(args);
    }

    void Logger::fatal(const std::string& message)
    {
        this->log(LoggerLevel::FATAL, message);
    }

}
