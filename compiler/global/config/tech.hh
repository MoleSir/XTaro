#pragma once

#include <string>
#include <json/json.hh>

namespace xtaro
{
    
    class Tech 
    {
    public:
        ///////////////////////////////////////////////////

        parse::Json spice{};
        parse::Json drc{};

        // Leafcell
        std::string bitcellSpicePath{};
        std::string dffSpicePath{};
        std::string prechargeSpicePath{};
        std::string senseampSpicePath{};
        std::string trigateSpicePath{};
        std::string writedriverSpicePath{};

        ///////////////////////////////////////////////////
    public:
        void load();
        static Tech* instance();

    private:
        Tech() = default;
        ~Tech() noexcept = default;
        Tech(const Tech&) = delete;
        Tech(Tech&&) = delete;
        Tech& operator = (const Tech&) = delete;
        Tech& operator = (Tech&&) = delete;        
    };

    extern Tech* tech;
}
