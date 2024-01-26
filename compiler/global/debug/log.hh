#pragma once

#include <string>
#include <fstream>
#include <array>

namespace xtaro
{

    class Log
    {
    public:
        Log(const std::string& filename);
        ~Log() noexcept;
        Log(const Log&) = delete;
        Log(Log&&) = delete;
        Log& operator = (const Log&) = delete;
        Log& operator = (Log&&) = delete;

    public:
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);
        void fatal(const std::string& message);

    private:
        void check() const;
        void log(const char* level, const std::string& message);
        void logDateTime();
        void logLevel(const char* level);

    private:
        std::ofstream _outfile{};
    };

}
