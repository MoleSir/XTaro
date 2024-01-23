#include "debug.hh"

#include <debug/logger.hh>
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

    void Debug::setReportWay(ReportWay way)
    {
        this->_reportWay = way;
    }

    static int operator | (ReportWay way1, ReportWay way2)
    {
        return static_cast<int>(way1) | static_cast<int>(way2); 
    }

    void Debug::reportError(
        const std::string& errorType, 
        const std::string& errorReason,
        ReportWay reportWay) const
    {
        std::string message {
            util::format("[%s] error! because: [%s]", errorType.c_str(), errorReason.c_str())
        };

        if (reportWay | ReportWay::LOG)
            logger->error(message);
        if (reportWay | ReportWay::CONSOLE)
            console->error(message);
        throw MessageException {std::move(message)};    
    }

    void Debug::reportError(
        const std::string& errorType, 
        const std::string& errorReason) const
    {
        this->reportError(errorType, errorType, this->_reportWay);
    }

    void Debug::exit(int code)
    {
        std::exit(code);
    }
}