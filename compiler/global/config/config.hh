#pragma once

#include <string>
#include <stdexcept>

namespace xtaro
{
    
    class Config
    {
    public:
        int         wordWidth{-1};
        int         addressWidth{-1};
        
        std::string techPath{"./tech/scn4m_subm"};
        std::string outputPath{"./output/"};
        std::string sramName{"sram"};

        // Folder path
        std::string sramFolderPath{};
        std::string simFolderPath{};

        // File path
        std::string spicePath{};
        std::string gdsiiPath{};
        std::string verilogPath{};


    public:
        void load(const std::string& configFile);

    public:
        static Config* instance();

    private:
        void buildOutputPath();
        void buildOutputFilesName();

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