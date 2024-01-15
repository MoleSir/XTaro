#pragma once

#include "string.hh"
#include <string>
#include <set>
#include <map>

namespace xtaro
{
    
    class StringPool
    {
    public:
        static StringPool* instance();

    public:
        String get(const std::string& str);

    private:
        StringPool() = default;
        ~StringPool() noexcept = default;
        StringPool(const StringPool&) = delete;
        StringPool(StringPool&&) = delete;
        StringPool& operator = (const StringPool&) = delete;
        StringPool& operator = (StringPool&&) = delete;

    private:
        std::set<std::string> _strings;
    };

    extern StringPool* stringPool;
}