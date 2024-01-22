#pragma

#include <string>
#include <fstream>
#include <array>

namespace xtaro
{
    enum class LoggerLevel
    {
        DEBUG = 0, 
        INFO, 
        WARNING, 
        ERROR, 
        FATAL, 
        SIZE,
    };
    #define LOGGER_LEVEL_SIZE (static_cast<std::size_t>(LoggerLevel::SIZE))

    class Logger
    {
    public:
        static Logger* instance();
    
    public:
        void open(const std::string& fileName);
        void close();
        void setLevel(LoggerLevel level);

    public:
        void debug(const char* fmt, ...);
        void debug(const std::string& message);

        void info(const char* fmt, ...);
        void info(const std::string& message);

        void warning(const char* fmt, ...);
        void warning(const std::string& message);

        void error(const char* fmt, ...); 
        void error(const std::string& message);

        void fatal(const char* fmt, ...);
        void fatal(const std::string& message);

    private:
        Logger();
        ~Logger() noexcept = default;
        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator = (const Logger&) = delete;
        Logger& operator = (Logger&&) = delete;

    private:
        void log(LoggerLevel level, const char* fmt, va_list args);
        void log(LoggerLevel level, const std::string& message);

        void logDateTime();
        void logLevel(LoggerLevel level);

    private:
        static std::array<const char*, LOGGER_LEVEL_SIZE> levelStrings;

    private:
        std::ofstream _outfile;
        LoggerLevel _level;
    };

    extern Logger* logger;
}
