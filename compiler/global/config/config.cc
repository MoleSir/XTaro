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
    Config* config{Config::instance()};

#define CONFIG_OPTIONAL_ARRTRIBUTION(attr, field, attrType)\
std::string field ## Name{attr};\
if (json.has(field ## Name))\
    this->field.assign( std::move(json[field ## Name].attrType()) );

#define CONFIG_NECESSARY_ARRTRIBUTION(attr, field, attrType)\
std::string field ## Name{attr};\
if (json.has(field ## Name))\
    this->field = json[field ## Name].attrType();\
else\
{\
    logger->error("'" attr "' not be given in config file.");\
    throw MessageException("Config", "'" attr "' not be given.");\
}

    void Config::load(const std::string& configFile)
    {
        parse::Json json{parse::Json::loadFromFile(configFile)};

        // Config Necessart arrtributions
        CONFIG_NECESSARY_ARRTRIBUTION("address_width", addressWidth, asInteger);
        CONFIG_NECESSARY_ARRTRIBUTION("word_width", wordWidth, asInteger);

        // Cinfig optional arrtributions
        CONFIG_OPTIONAL_ARRTRIBUTION("tech_path", techPath, asString);
        this->techPath.assign(util::absolutePath(this->techPath));
        if (this->techPath.back() != '/')
            this->techPath.push_back('/');

        CONFIG_OPTIONAL_ARRTRIBUTION("output_path", outputPath, asString);
        this->outputPath = util::absolutePath(this->outputPath);
        if (this->outputPath.back() != '/')
            this->outputPath.push_back('/');

        CONFIG_OPTIONAL_ARRTRIBUTION("sram_name", sramName, asString);

        // Build output file path and build folder to hold them
        this->buildOutputFilesName();
        this->buildOutputPath();
    }

    void Config::buildOutputPath()
    {
        if (!util::directoryExists(this->outputPath))
        {
            if (!std::filesystem::create_directories(this->outputPath))
                throw MessageException(
                    "Load Config", 
                    util::format("Create output path '%s'", this->outputPath.c_str())
                );
        }

        std::filesystem::create_directories(this->sramFolderPath);
        std::filesystem::create_directories(this->simFolderPath);
    }

    void Config::buildOutputFilesName()
    {
        this->sramFolderPath = this->outputPath + "sram/";
        this->simFolderPath = this->outputPath + "simulation/";

        this->spicePath = util::format("%s%s.sp", this->sramFolderPath.c_str(), this->sramName.c_str());
        this->gdsiiPath = util::format("%s%s.gds", this->sramFolderPath.c_str(), this->sramName.c_str());
        this->verilogPath = util::format("%s%s.v", this->sramFolderPath.c_str(), this->sramName.c_str());
    }

    Config* Config::instance()
    {
        static Config _config{};
        return &_config;
    }   
}