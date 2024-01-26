#include "debug.hh"

#include <debug/log.hh>
#include <debug/console.hh>
#include <debug/exception.hh>

#include <util/format.hh>

#include <cstdlib>

namespace xtaro
{
    Debug* debug {Debug::instance()};

    Debug* Debug::instance()
    {
        static Debug _debug;
        return &_debug;
    }

    void Debug::setLevel(DebugLevel way)
    {
        this->_level = way;
    }

    void Debug::init(const std::string& logfile)
    {
        this->_log = std::make_unique<Log>(logfile);
        this->_console = std::make_unique<Console>();
    }

    void Debug::debug(const char* fmt, ...)
    {
        if (!this->check(DebugLevel::DEBUG)) return;

        va_list(args);
        va_start(args, fmt);
        std::string message {util::vformat(fmt, args)};
        va_end(args);

        this->debug(message);
    }

    void Debug::debug(const std::string& message)
    {
        if (!this->check(DebugLevel::DEBUG)) return;

        this->_log->debug(message);
        this->_console->debug(message);
    }

    void Debug::info(const char* fmt, ...)
    {
        if (!this->check(DebugLevel::INFO)) return;

        va_list(args);
        va_start(args, fmt);
        std::string message {util::vformat(fmt, args)};
        va_end(args);

        this->info(message);
    }

    void Debug::info(const std::string& message)
    {
        if (!this->check(DebugLevel::INFO)) return;

        this->_log->info(message);
        this->_console->info(message);
    }

    void Debug::warning(const char* fmt, ...)
    {
        if (!this->check(DebugLevel::WARNING)) return;

        va_list(args);
        va_start(args, fmt);
        std::string message {util::vformat(fmt, args)};
        va_end(args);

        this->warning(message);
    }
 
    void Debug::warning(const std::string& message)
    {
        if (!this->check(DebugLevel::WARNING)) return;

        this->_log->warning(message);
        this->_console->warning(message);
    }
    
    void Debug::error(const std::string& message)
    {
        if (this->check(DebugLevel::ERROR))
        {
            this->_log->error(message);
            this->_console->error(message);
        }
        throw MessageException{message};
    }

    void Debug::error(const std::string& type, const std::string& reason)
    {
        if (this->check(DebugLevel::ERROR))
        {
            std::string message {
                util::format("[%s] error, because: [%s]", type.c_str(), reason.c_str())
            };
            this->_log->error(message);
            this->_console->error(message);

            throw MessageException{std::move(message)};
        }
        throw TypeReasonException{type, reason};
    }

    bool Debug::check(DebugLevel level) const
    {
        this->checkInit();
        return this->checkLevel(level);
    }

    void Debug::checkInit() const
    {
        if (this->_log.get() == nullptr || this->_console.get() == nullptr)
            throw TypeReasonException{"Debug Message", "'debug' object is not be initialized"};
    }

    bool Debug::checkLevel(DebugLevel level) const
    {
        return this->_level <= level;
    }

}