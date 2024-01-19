#pragma once

#include <string>
#include <json/json.hh>

namespace xtaro
{
    
    class Tech 
    {
    public:
        std::string techpath{};
        parse::Json spice{};
        parse::Json drc{};

        void load();

    public:
        static Tech* instance();

    private:
        Tech() = default;
        ~Tech() noexcept = default;
        Tech(const Tech&) = delete;
        Tech(Tech&&) = delete;
        Tech& operator = (const Tech&) = delete;
        Tech& operator = (Tech&&) = delete;

        void checkSpiceMessage() const;
        void checkDRCMessage() const;
        void checkTechFiles() const;
        
    };

    extern Tech* tech;
}
