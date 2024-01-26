#include "log.hh"

#include "exception.hh"
#include <util/format.hh>

#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

namespace xtaro
{

    Log::Log(const std::string& filename):
        _outfile{filename}
    {
        if (!this->_outfile.is_open())
            throw TypeReasonException {
                "Create Log", util::format("Open '%s' failied", filename.c_str())
            };
    }

    Log::~Log() noexcept
    {
        this->_outfile.close();
    }

    void Log::check() const
    {
        if (!this->_outfile.is_open())
            throw TypeReasonException {
                "Log", "Log file does not open"
            };
    }

    void Log::log(const char* level, const std::string& message)
    {
        this->logDateTime();
        this->logLevel(level);
        this->_outfile << message << '\n';

        this->_outfile.flush();
    }

    void Log::logDateTime()
    {
        time_t current_ticks = std::time(NULL);
        struct tm* time = std::localtime(&current_ticks);

        char dateTime[32] = {0};
        std::memset(dateTime, 0, sizeof(dateTime));
        std::strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", time);
        this->_outfile << '[' << dateTime << ']';
    }
    
    void Log::logLevel(const char* level)
    {
        this->_outfile << ' ' << level << " > ";
    }

    void Log::debug(const std::string& message)
    {
        this->check();
        this->log(" DEBUG ", message);
    }

    void Log::info(const std::string& message)
    {
        this->check();
        this->log(" INFO  ", message);
    }

    void Log::warning(const std::string& message)
    {
        this->check();
        this->log("WARNING", message);
    }

    void Log::error(const std::string& message)
    {
        this->check();
        this->log(" ERROR ", message);
    }

    void Log::fatal(const std::string& message)
    {
        this->check();
        this->log(" FATAL ", message);
    }

}
