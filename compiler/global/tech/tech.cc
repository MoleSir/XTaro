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

    static void checkTechFilesComplete();
    static void assignFilesPath();
    static void loadTechMessage();
    static void checkTechMessage();

    Tech* tech{Tech::instance()};

    void Tech::load()
    {
        // Four steps:
        checkTechFilesComplete();

        assignFilesPath();

        loadTechMessage();
        
        checkTechMessage();
    }

    // ============================ Check Tech Files Complete =============================== //

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

    static void noExitsError(const std::string& path)
    {
        logger->error(util::format("'%s' not be given in tech lib.", path.c_str()));
        throw MessageException("Load tech", util::format("'%s' not be given.", path.c_str()));
    }

    static void checkDirectoryExits(const std::string& directory)
    {
        if (!util::directoryExists(directory))
            noExitsError(directory);
    }

    static void checkFileExits(const std::string& file)
    {
        if (!util::fileExists(file))
            noExitsError(file);
    }

    static void checkTechFilesComplete()
    {
        // Check spice file
        std::string spicepath = config->techPath + "spice/";

        checkDirectoryExits(spicepath);
        checkFileExits(spicepath + "bitcell.sp");
        checkFileExits(spicepath + "dff.sp");
        checkFileExits(spicepath + "precharge.sp");
        checkFileExits(spicepath + "sense_amp.sp");
        checkFileExits(spicepath + "tri_gate.sp");
        checkFileExits(spicepath + "write_driver.sp");
    }

    // ============================ Assign Files Path =============================== //

    static void assignSpicefilePath()
    {
        tech->bitcellSpicePath = config->techPath + "spice/bitcell.sp";
        tech->dffSpicePath = config->techPath + "spice/dff.sp";
        tech->prechargeSpicePath = config->techPath + "spice/precharge.sp";
        tech->senseampSpicePath = config->techPath + "spice/sense_amp.sp";
        tech->trigateSpicePath = config->techPath + "spice/tri_gate.sp";
        tech->writedriverSpicePath = config->techPath + "spice/write_driver.sp";
    }

    static void assignGDSIIfilePath()
    {
    }

    static void assignFilesPath()
    {
        assignSpicefilePath();
        assignGDSIIfilePath();
    }

    // ============================ Load Tech Message =============================== //

    static void loadTechMessage()
    {
        parse::Json json{ parse::Json::loadFromFile(config->techPath + "tech.json") };

        // Spice tech
        tech->spice = json.get("spice");
        if (tech->spice.invalid())
            throw MessageException("Load tech", "No spice tech message.");

        tech->drc = json.get("drc");
        if (tech->drc.invalid())
            throw MessageException("Load tech", "No drc tech message.");
    }

    // ============================ Check Tech Message =============================== //

    static void checkSpiceMessage()
    {
        if (!tech->spice.has("nmos")) throw MessageException("Load spice tech", "No 'nmos'.");
        if (!tech->spice.has("pmos")) throw MessageException("Load spice tech", "No 'pmos'.");
    }

    static void checkDRCMessage()
    {
        if (!tech->drc.has("minwidth_poly")) throw MessageException("Load drc tech", "No 'minwidth_poly'.");
    }

    static void checkTechMessage()
    {
        checkSpiceMessage();
        checkDRCMessage();
    }

    // ============================ Tech::instance =============================== //

    Tech* Tech::instance()
    {
        static Tech _tech{};
        return &_tech;
    }
    
}