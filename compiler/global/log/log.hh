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
    
        static Logger& instance() noexcept;
        void open(const std::string& fileName) noexcept;
        void close() noexcept;

        void setLevel(Logger::Level level) noexcept
        { this->_level = level; }

        void log(Logger::Level level, const char* format, va_list args);
        void debug(const char* format, ...);
        void info(const char* format, ...);
        void warning(const char* format, ...);
        void error(const char* format, ...); 
        void fatal(const char* format, ...);

    private:
        Logger() noexcept;
        ~Logger() noexcept;

    private:
        std::ofstream _fileOut;
        Logger::Level _level;

    private:
        static const char* _levelStrings[Level::COUNT];
        static char _buffer[MAX_BUFFER_SIZE];
    };

    extern Logger& logger;
}
