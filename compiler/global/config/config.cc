#include "config.hh"

#include <exception/msgexception.hh>
#include <log/logger.hh>
#include <json/json.hh>
#include <util/util.hh>

#include <string>
#include <iostream>
#include <filesystem>

namespace xtaro
{
    Config* config{Config::instance()};

    void Config::load(const std::string& configFile)
    {
        parse::Json json{parse::Json::loadFromFile(configFile)};

        std::string wordWidthName{"word_width"};
        if (json.has(wordWidthName)) 
            this->wordWidth = json[wordWidthName].asInteger();
        else
        {
            logger->error("'Word Width' must be given in config file.");
            throw MessageException("Config", "'Word Width' must be given.");
        }

        std::string addressWidthName{"address_width"};
        if (json.has(addressWidthName)) 
            this->addressWidth = json[addressWidthName].asInteger();
        else
        {
            logger->error("'Address Width' must be given in config file.");
            throw MessageException("Config", "'Address Width' must be given.");
        }

        std::string techPathName{"tech_path"};
        if (json.has(techPathName))
            this->techPath.assign( std::move(json[techPathName].asString()) );
        
        std::string techNameName{"tech_name"};
        if (json.has(techNameName))
            this->techName.assign( std::move(json[techNameName].asString()) );

        std::string outputPathName{"output_path"};
        if (json.has(outputPathName))
            this->outputPath.assign( std::move(json[outputPathName].asString()) );
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
    }

    Config* Config::instance()
    {
        static Config _config{};
        return &_config;
    }   
}