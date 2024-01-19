#pragma

#include <string>
#include <fstream>
#include <stdexcept>

namespace xtaro
{
    
    class Logger
    {
    public:
        enum 
        {
            MAX_BUFFER_SIZE = 256,
        };

        enum Level
        {
            DEBUG = 0, 
            INFO, 
            WARNING, 
            ERROR, 
            FATAL, 
            COUNT,
        };
    
        static Logger* instance() noexcept;
    
    public:
        void open(const std::string& fileName);
        void close();

        void setLevel(Logger::Level level) noexcept
        { this->_level = level; }

    public:
        void debug(const char* format, ...);
        void debug(const std::string& message);

        void info(const char* format, ...);
        void info(const std::string& message);

        void warning(const char* format, ...);
        void warning(const std::string& message);

        void error(const char* format, ...); 
        void error(const std::string& message);

        void fatal(const char* format, ...);
        void fatal(const std::string& message);

    private:
        Logger();
        ~Logger() noexcept = default;
        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator = (const Logger&) = delete;
        Logger& operator = (Logger&&) = delete;

    private:
        static const char* _levelStrings[Level::COUNT];
        static char _buffer[MAX_BUFFER_SIZE];

    private:
        void log(Logger::Level level, const char* format, va_list args);

    private:
        std::ofstream _fileOut;
        Logger::Level _level;
    };

    extern Logger* logger;
}
