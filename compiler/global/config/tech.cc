#include "tech.hh"
#include <config/option.hh>
#include <debug/logger.hh>
#include <debug/debug.hh>
#include <util/format.hh>
#include <util/file.hh>
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

    static void noExitsError(const std::string& path)
    {
        debug->reportError("Load Tech", util::format("'%s' not be given."));
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
        std::string spicepath = option->techPath + "spice/";

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
        tech->bitcellSpicePath = option->techPath + "spice/bitcell.sp";
        tech->dffSpicePath = option->techPath + "spice/dff.sp";
        tech->prechargeSpicePath = option->techPath + "spice/precharge.sp";
        tech->senseampSpicePath = option->techPath + "spice/sense_amp.sp";
        tech->trigateSpicePath = option->techPath + "spice/tri_gate.sp";
        tech->writedriverSpicePath = option->techPath + "spice/write_driver.sp";
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
        parse::Json json{ parse::Json::loadFromFile(option->techPath + "tech.json") };

        // Spice tech
        tech->spice = json.get("spice");
        if (tech->spice.invalid())
            debug->reportError("Load tech", "No spice tech message.");

        tech->drc = json.get("drc");
        if (tech->drc.invalid())
            debug->reportError("Load tech", "No drc tech message.");
    }

    // ============================ Check Tech Message =============================== //

    static void checkSpiceMessage()
    {
        if (!tech->spice.has("nmos")) debug->reportError("Load spice tech", "No 'nmos'.");
        if (!tech->spice.has("pmos")) debug->reportError("Load spice tech", "No 'pmos'.");
    }

    static void checkDRCMessage()
    {
        if (!tech->drc.has("minwidth_poly")) debug->reportError("Load drc tech", "No 'minwidth_poly'.");
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