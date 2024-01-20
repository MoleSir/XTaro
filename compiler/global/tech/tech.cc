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
        std::string spicepath = config->techPath + "spice";

        if (!util::directoryExists(spicepath))
            throw MessageException("Tech invaild", "No spice file folder.");
        if (!util::fileExists(spicepath + "/bitcell.sp"))
            throw MessageException("Tech invalid", "No 'bitcell.sp'");
        if (!util::fileExists(spicepath + "/dff.sp"))
            throw MessageException("Tech invalid", "No 'dff.sp'");
        if (!util::fileExists(spicepath + "/precharge.sp"))
            throw MessageException("Tech invalid", "No 'precharge.sp'");
        if (!util::fileExists(spicepath + "/sense_amp.sp"))
            throw MessageException("Tech invalid", "No 'sense_amp.sp'");
        if (!util::fileExists(spicepath + "/tri_gate.sp"))
            throw MessageException("Tech invalid", "No 'tri_gate.sp'");
        if (!util::fileExists(spicepath + "/write_driver.sp"))
            throw MessageException("Tech invalid", "No 'write_driver.sp'");
    }

    void Tech::load()
    {
        // Get tech file path and load it

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

    Tech* Tech::instance()
    {
        static Tech _tech{};
        return &_tech;
    }
    
}