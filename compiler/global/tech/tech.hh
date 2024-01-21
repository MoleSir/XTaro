#pragma once

#include <string>
#include <json/json.hh>

namespace xtaro
{
    
    class Tech 
    {
    public:
        parse::Json spice{};
        parse::Json drc{};

        // Leafcell
        std::string bitcellSpicePath{};
        std::string dffSpicePath{};
        std::string prechargeSpicePath{};
        std::string senseampSpicePath{};
        std::string trigateSpicePath{};
        std::string writedriverSpicePath{};

    public:
        void load();

    public:
        static Tech* instance();

    private:
        static void checkDirectoryExits(const std::string& directory);
        static void checkFileExits(const std::string& file);
        static void noExitsError(const std::string& path);

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
