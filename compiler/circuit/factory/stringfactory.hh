#pragma once

#include <string>
#include <string_view>
#include <set>

namespace xtaro::circuit
{

    class StringFactory
    {
    private:
        StringFactory() = default;
        ~StringFactory() noexcept = default;
        StringFactory(const StringFactory&) = delete;
        StringFactory(StringFactory&&) noexcept = delete;
        StringFactory& operator = (const StringFactory&) = delete;
        StringFactory& operator = (StringFactory&&) noexcept = delete;

    public:
        static StringFactory* instance();

    public:
        std::string_view get(const char* fmt, ...);
        std::string_view get(std::string str);
        std::string_view get(const char* str);

    private:
        std::set<std::string> _stringPool {};
    };

    extern StringFactory* stringFactory;

}