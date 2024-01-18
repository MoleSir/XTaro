#pragma once

#include <string>
#include <stdexcept>

namespace xtaro
{
    
    class Config
    {
    public:
        std::int32_t wordWidth{-1};
        std::int32_t addressWidth{-1};
        
        std::string  techPath{"./tech"};
        std::string  techName{"scn4m_subm"};
        
        std::string  outputPath{"./output"};
        std::string  sramName{"sram"};

        void load(const std::string& configFile);

    public:
        static Config* instance();

    private:
        Config() = default;
        ~Config() noexcept = default;
        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator = (const Config&) = delete;
        Config& operator = (Config&&) = delete;
    };

    extern Config* config;
}