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
        void checkTechFiles() const;

        static bool fileExists(const std::string& filePath);
        static bool directoryExists(const std::string& dirPath);
    };

    extern Tech* tech;
}
