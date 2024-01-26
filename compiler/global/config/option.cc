#include "option.hh"

#include <debug/debug.hh>

#include <json/json.hh>
#include <util/format.hh>
#include <util/file.hh>

#include <string>
#include <iostream>
#include <filesystem>

namespace xtaro
{

    static void buildOutputPath();
    static void buildOutputFilesName();

    static void configNecessaryAttributions(parse::Json& json);
    static void configOptionalAttributions(parse::Json& json);
    static void configPVTList(parse::Json& json);

    Option* option{Option::instance()};

    void Option::load(const std::string& optionFile)
    {
        parse::Json json{parse::Json::loadFromFile(optionFile)};

        // Option attributions
        configNecessaryAttributions(json);
        configOptionalAttributions(json);
        configPVTList(json);

        // Build output file path and build folder to hold them
        buildOutputFilesName();
        buildOutputPath();
    }

    // ========================== Option Necessary Attributions ========================== //

    #define CONFIG_NECESSARY_ATTRIBUTION(attr, field, attrType)\
    parse::Json field ## Json {json.get(attr)};\
    if (!field ## Json.invalid())\
        option->field = std::move(field ## Json.attrType());\
    else\
        debug->error("Load Option", "'" attr "' not be given.");

    static void configNecessaryAttributions(parse::Json& json)
    {
        CONFIG_NECESSARY_ATTRIBUTION("address_width", addressWidth, asInteger);
        CONFIG_NECESSARY_ATTRIBUTION("word_width", wordWidth, asInteger);
    }

    // ========================== Option Optional Attributions ========================== //

    #define CONFIG_OPTIONAL_ATTRIBUTION(attr, field, attrType)\
    parse::Json field ## Json {json.get(attr)};\
    if (!field ## Json.invalid())\
        option->field = std::move(field ## Json.attrType());

    static void configOptionalAttributions(parse::Json& json)
    {
        CONFIG_OPTIONAL_ATTRIBUTION("tech_path", techPath, asString);
        option->techPath.assign(util::absolutePath(option->techPath));
        if (option->techPath.back() != '/')
            option->techPath.push_back('/');

        CONFIG_OPTIONAL_ATTRIBUTION("output_path", outputPath, asString);
        option->outputPath = util::absolutePath(option->outputPath);
        if (option->outputPath.back() != '/')
            option->outputPath.push_back('/');

        CONFIG_OPTIONAL_ATTRIBUTION("sram_name", sramName, asString);
    }

    // ========================== Option PVT List ========================== //

    static void configPVTList(parse::Json& json)
    {
        parse::Json processCorners {json.get("process_corners")};
        if (!processCorners.invalid())
        {
            for (std::size_t i = 0; i < processCorners.size(); ++i)
                option->processCorners.emplace_back( processCorners[i].asString() );
        }

        parse::Json supplyVoltages {json.get("supply_voltages")};
        if (!supplyVoltages.invalid())
        {
            for (std::size_t i = 0; i < supplyVoltages.size(); ++i)
                option->supplyVoltages.emplace_back( supplyVoltages[i].asDecimal() );
        }

        parse::Json temperatures {json.get("temperatures")};
        if (!temperatures.invalid())
        {
            for (std::size_t i = 0; i < temperatures.size(); ++i)
                option->temperatures.emplace_back( temperatures[i].asDecimal() );
        }
    }

    static void buildOutputPath()
    {
        if (!util::directoryExists(option->outputPath))
        {
            if (!std::filesystem::create_directories(option->outputPath))
                debug->error(
                    "Load Option", 
                    util::format("Create output path '%s'", option->outputPath.c_str())
                );
        }

        std::filesystem::create_directories(option->sramFolderPath);
        std::filesystem::create_directories(option->simFolderPath);
    }

    static void buildOutputFilesName()
    {
        option->sramFolderPath = option->outputPath + "sram/";
        option->simFolderPath = option->outputPath + "simulation/";

        option->spicePath = util::format("%s%s.sp", option->sramFolderPath.c_str(), option->sramName.c_str());
        option->gdsiiPath = util::format("%s%s.gds", option->sramFolderPath.c_str(), option->sramName.c_str());
        option->verilogPath = util::format("%s%s.v", option->sramFolderPath.c_str(), option->sramName.c_str());
    }

    Option* Option::instance()
    {
        static Option _option{};
        return &_option;
    }   
}