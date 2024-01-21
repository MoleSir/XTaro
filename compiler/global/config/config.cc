#include "config.hh"

#include <exception/msgexception.hh>
#include <debug/logger.hh>
#include <json/json.hh>
#include <util/format.hh>

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

    Config* config{Config::instance()};

    void Config::load(const std::string& configFile)
    {
        parse::Json json{parse::Json::loadFromFile(configFile)};

        // Config attributions
        configNecessaryAttributions(json);
        configOptionalAttributions(json);
        configPVTList(json);

        // Build output file path and build folder to hold them
        buildOutputFilesName();
        buildOutputPath();
    }

    // ========================== Config Necessary Attributions ========================== //

    #define CONFIG_NECESSARY_ATTRIBUTION(attr, field, attrType)\
    parse::Json field ## Json {json.get(attr)};\
    if (!field ## Json.invalid())\
        config->field = std::move(field ## Json.attrType());\
    else\
    {\
        logger->error("'" attr "' not be given in config file.");\
        throw MessageException("Config", "'" attr "' not be given.");\
    }

    static void configNecessaryAttributions(parse::Json& json)
    {
        CONFIG_NECESSARY_ATTRIBUTION("address_width", addressWidth, asInteger);
        CONFIG_NECESSARY_ATTRIBUTION("word_width", wordWidth, asInteger);
    }

    // ========================== Config Optional Attributions ========================== //

    #define CONFIG_OPTIONAL_ATTRIBUTION(attr, field, attrType)\
    parse::Json field ## Json {json.get(attr)};\
    if (!field ## Json.invalid())\
        config->field = std::move(field ## Json.attrType());

    static void configOptionalAttributions(parse::Json& json)
    {
        CONFIG_OPTIONAL_ATTRIBUTION("tech_path", techPath, asString);
        config->techPath.assign(util::absolutePath(config->techPath));
        if (config->techPath.back() != '/')
            config->techPath.push_back('/');

        CONFIG_OPTIONAL_ATTRIBUTION("output_path", outputPath, asString);
        config->outputPath = util::absolutePath(config->outputPath);
        if (config->outputPath.back() != '/')
            config->outputPath.push_back('/');

        CONFIG_OPTIONAL_ATTRIBUTION("sram_name", sramName, asString);
    }

    // ========================== Config PVT List ========================== //

    static void configPVTList(parse::Json& json)
    {
        parse::Json processCorners {json.get("process_corners")};
        if (!processCorners.invalid())
        {
            for (std::size_t i = 0; i < processCorners.size(); ++i)
                config->processCorners.emplace_back( processCorners[i].asString() );
        }

        parse::Json supplyVoltages {json.get("supply_voltages")};
        if (!supplyVoltages.invalid())
        {
            for (std::size_t i = 0; i < supplyVoltages.size(); ++i)
                config->supplyVoltages.emplace_back( supplyVoltages[i].asDecimal() );
        }

        parse::Json temperatures {json.get("temperatures")};
        if (!temperatures.invalid())
        {
            for (std::size_t i = 0; i < temperatures.size(); ++i)
                config->temperatures.emplace_back( temperatures[i].asDecimal() );
        }
    }

    static void buildOutputPath()
    {
        if (!util::directoryExists(config->outputPath))
        {
            if (!std::filesystem::create_directories(config->outputPath))
                throw MessageException(
                    "Load Config", 
                    util::format("Create output path '%s'", config->outputPath.c_str())
                );
        }

        std::filesystem::create_directories(config->sramFolderPath);
        std::filesystem::create_directories(config->simFolderPath);
    }

    static void buildOutputFilesName()
    {
        config->sramFolderPath = config->outputPath + "sram/";
        config->simFolderPath = config->outputPath + "simulation/";

        config->spicePath = util::format("%s%s.sp", config->sramFolderPath.c_str(), config->sramName.c_str());
        config->gdsiiPath = util::format("%s%s.gds", config->sramFolderPath.c_str(), config->sramName.c_str());
        config->verilogPath = util::format("%s%s.v", config->sramFolderPath.c_str(), config->sramName.c_str());
    }

    Config* Config::instance()
    {
        static Config _config{};
        return &_config;
    }   
}