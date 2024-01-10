#include "config.hh"

#include <exception/msgexception.hh>
#include <json/json.hh>

#include <string>

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
            throw MessageException("Config", "'Word Width' must be given.");

        std::string addressWidthName{"address_width"};
        if (json.has(addressWidthName)) 
            this->addressWidth = json[addressWidthName].asInteger();
        else
            throw MessageException("Config", "'Address Width' must be given.");

        std::string techPathName{"tech_path"};
        if (json.has(techPathName))
            this->techPath.assign( std::move(json[techPathName].asString()) );
        
        std::string techNameName{"tech_name"};
        if (json.has(techNameName))
            this->techName.assign( std::move(json[techNameName].asString()) );

        std::string outputPathName{"output_path"};
        if (json.has(outputPathName))
            this->outputPath.assign( std::move(json[outputPathName].asString()) );
    }

    Config* Config::instance()
    {
        static Config _config{};
        return &_config;
    }   
}