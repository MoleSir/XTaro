#include "tech.hh"
#include <config/config.hh>
#include <util/util.hh>
#include <exception/msgexception.hh>
#include <json/json.hh>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace xtaro
{
    Tech* tech{Tech::instance()};

    void Tech::checkTechFiles() const
    {
        // Check spice file
        std::string spicepath = this->techpath + "spice";
        if (!Tech::directoryExists(spicepath))
            throw MessageException("Tech invaild", "No spice file folder.");
        if (!Tech::fileExists(spicepath + "/bitcell.sp"))
            throw MessageException("Tech invalid", "No 'bitcell.sp'");
        if (!Tech::fileExists(spicepath + "/dff.sp"))
            throw MessageException("Tech invalid", "No 'dff.sp'");
        if (!Tech::fileExists(spicepath + "/sense_amp.sp"))
            throw MessageException("Tech invalid", "No 'sense_amp.sp'");
        if (!Tech::fileExists(spicepath + "/tri_gate.sp"))
            throw MessageException("Tech invalid", "No 'tri_gate.sp'");
        if (!Tech::fileExists(spicepath + "/write_driver.sp"))
            throw MessageException("Tech invalid", "No 'write_driver.sp'");
    }

    void Tech::load()
    {
        // Get tech file path and load it
        this->techpath = util::format("%s%s/", 
                                      config->techPath.c_str(), 
                                      config->techName.c_str());

        // Check tech complete
        this->checkTechFiles();

        parse::Json json{ parse::Json::loadFromFile(this->techpath + "tech.json") };

        // Spice tech
        this->spice = json.get("spice");
        if (this->spice.invalid())
            throw MessageException("Load tech", "No spice tech message.");

        this->checkSpiceMessage();        
    }
    
    void Tech::checkSpiceMessage() const
    {
        if (!this->spice.has("nmos")) throw MessageException("Load spice tech", "No 'nmos' message.");
        if (!this->spice.has("pmos")) throw MessageException("Load spice tech", "No 'pmos' message.");
    }

    Tech* Tech::instance()
    {
        static Tech _tech{};
        return &_tech;
    }  

    bool Tech::fileExists(const std::string& filePath) 
    {
        return std::filesystem::exists(filePath) && 
               std::filesystem::is_regular_file(filePath);
    }

    bool Tech::directoryExists(const std::string& dirPath) 
    {
        return std::filesystem::exists(dirPath) && 
               std::filesystem::is_directory(dirPath);
    }
}