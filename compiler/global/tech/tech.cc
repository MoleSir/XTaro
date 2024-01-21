#include "tech.hh"
#include <config/config.hh>
#include <debug/logger.hh>
#include <util/format.hh>
#include <exception/msgexception.hh>
#include <json/json.hh>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace xtaro
{
    Tech* tech{Tech::instance()};

#define CHECK_Directory(directory)\
if (!util::fileExists(spicepath + file))\
{\
    logger->error("'" file "' not be given in tech lib.");\
    throw MessageException("Tech", "'" file "' not be given.");\
}

#define CHECK_FILE(directory, file)\
if (!util::fileExists(file))\
{\
    logger->error("'" file "' not be given in tech lib.");\
    throw MessageException("Tech", "'" file "' not be given.");\
}

    void Tech::checkTechFiles() const
    {
        // Check spice file
        std::string spicepath = config->techPath + "spice/";

        Tech::checkDirectoryExits(spicepath);
        Tech::checkFileExits(spicepath + "bitcell.sp");
        Tech::checkFileExits(spicepath + "dff.sp");
        Tech::checkFileExits(spicepath + "precharge.sp");
        Tech::checkFileExits(spicepath + "sense_amp.sp");
        Tech::checkFileExits(spicepath + "tri_gate.sp");
        Tech::checkFileExits(spicepath + "write_driver.sp");
    }

    void Tech::load()
    {
        // Check tech complete
        this->checkTechFiles();

        this->bitcellSpicePath = config->techPath + "spice/bitcell.sp";
        this->dffSpicePath = config->techPath + "spice/dff.sp";
        this->prechargeSpicePath = config->techPath + "spice/precharge.sp";
        this->senseampSpicePath = config->techPath + "spice/sense_amp.sp";
        this->trigateSpicePath = config->techPath + "spice/tri_gate.sp";
        this->writedriverSpicePath = config->techPath + "spice/write_driver.sp";

        parse::Json json{ parse::Json::loadFromFile(config->techPath + "tech.json") };

        // Spice tech
        this->spice = json.get("spice");
        if (this->spice.invalid())
            throw MessageException("Load tech", "No spice tech message.");

        this->drc = json.get("drc");
        if (this->drc.invalid())
            throw MessageException("Load tech", "No drc tech message.");

        this->checkSpiceMessage();
        this->checkDRCMessage(); 
    }
    
    void Tech::checkSpiceMessage() const
    {
        if (!this->spice.has("nmos")) throw MessageException("Load spice tech", "No 'nmos'.");
        if (!this->spice.has("pmos")) throw MessageException("Load spice tech", "No 'pmos'.");
    }

    void Tech::checkDRCMessage() const
    {
        if (!this->drc.has("minwidth_poly")) throw MessageException("Load drc tech", "No 'minwidth_poly'.");
    }

    void Tech::checkDirectoryExits(const std::string& directory)
    {
        if (!util::directoryExists(directory))
            Tech::noExitsError(directory);
    }

    void Tech::checkFileExits(const std::string& file)
    {
        if (!util::fileExists(file))
            Tech::noExitsError(file);
    }

    void Tech::noExitsError(const std::string& path)
    {
        logger->error(util::format("'%s' not be given in tech lib.", path.c_str()));
        throw MessageException("Load tech", util::format("'%s' not be given.", path.c_str()));
    }

    Tech* Tech::instance()
    {
        static Tech _tech{};
        return &_tech;
    }
    
}