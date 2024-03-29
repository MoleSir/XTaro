#pragma once

#include <string>
#include <memory>

namespace xtaro
{
    
    class Log;
    class Console;

    enum class DebugLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
    };

    class Debug
    {
    public:
        static Debug* instance();

    public:
        void init(const std::string& logfile);

    public:
        void debug(const char* fmt, ...);
        void debug(const std::string& message);

        void info(const char* fmt, ...);
        void info(const std::string& message);
    
        void warning(const char* fmt, ...);
        void warning(const std::string& message);
    
        void error(const char* fmt, ...);
        void error(const std::string& message);
        void errorWithException(const std::string& type, const std::string& reason);

        void fatal(int status, const char* fmt, ...);
        void fatal(int status, const std::string& message);

    public:
        void setLevel(DebugLevel level);

    private:
        bool check(DebugLevel level) const;
        bool checkLevel(DebugLevel level) const;
        void checkInit() const;

    private:
        Debug() = default;
        ~Debug() noexcept = default;
        Debug(const Debug&) = delete;
        Debug(Debug&&) = delete;
        Debug& operator = (const Debug&) = delete;
        Debug& operator = (Debug&&) = delete;
    
    private:
        DebugLevel _level{DebugLevel::DEBUG};

        std::unique_ptr<Log> _log{nullptr};
        std::unique_ptr<Console> _console{nullptr};
    };

    extern Debug* debug;

}