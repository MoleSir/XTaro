#pragma once

#include <string>

namespace xtaro
{
    enum class ReportWay
    {
        NONE = 0,

        LOG       = (0x1 << 1), 
        CONSOLE   = (0x1 << 2),

        ALL = (LOG | CONSOLE)
    };

    class Debug
    {
    public:
        static Debug* instance();

    public:
        void setReportWay(ReportWay way);

    public:
        void reportError(
             const std::string& errorType, 
             const std::string& errorReason,
             ReportWay reportWay) const;

        void reportError(
             const std::string& errorType, 
             const std::string& errorReason) const;

    public: 
        void exit(int code);

    private:
        Debug() = default;
        ~Debug() noexcept = default;
        Debug(const Debug&) = delete;
        Debug(Debug&&) = delete;
        Debug& operator = (const Debug&) = delete;
        Debug& operator = (Debug&&) = delete;
    
    private:
        ReportWay _reportWay{ReportWay::ALL};
    };

    extern Debug* debug;

}