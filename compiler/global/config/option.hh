#pragma once

#include <string>
#include <vector>

namespace xtaro
{
    
    class Option
    {
    public:
    ////////////////////////////////////////////////////////
    
        int         wordWidth{-1};
        int         addressWidth{-1};
        
        std::string techPath{"./tech/scn4m_subm"};
        std::string outputPath{"./output/"};
        std::string sramName{"sram"};

        // PVT
        std::vector<std::string> processCorners{};
        std::vector<double>      supplyVoltages{};
        std::vector<double>      temperatures{};

        // Folder path
        std::string sramFolderPath{};
        std::string libFolderPath{};
        std::string simFolderPath{};

        // File path
        std::string spicePath{};
        std::string gdsiiPath{};
        std::string verilogPath{};

    /////////////////////////////////////////////////////////
    public:
        void load(const std::string& configFile);
        static Option* instance();

    private:
        Option() = default;
        ~Option() noexcept = default;
        Option(const Option&) = delete;
        Option(Option&&) = delete;
        Option& operator = (const Option&) = delete;
        Option& operator = (Option&&) = delete;
    };

    extern Option* option;
}