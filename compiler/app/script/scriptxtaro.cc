#include "scriptxtaro.hh"

#include <config/option.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

#include <iostream>
#include <memory>

namespace xtaro
{

    ScriptXTaro* ScriptXTaro::instance()
    {
        static ScriptXTaro _xtaro;
        return &_xtaro;
    }

    void ScriptXTaro::init(const std::string& optionFile)
    {
        try
        {
            debug->info("Load Option...");
            option->load(optionFile);
            debug->info("Load Option successfully!");

            debug->info("Load Tech...");
            tech->load();
            debug->info("Load Tech successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("Init XTaro failed~");
            std::exit(12);
        }
    }

    void ScriptXTaro::createSRAM()
    {
        try
        {
            debug->info("Generate SRAM circuit...");
            circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
            this->_sram = std::make_unique<circuit::SRAM>(option->sramName, &argument);
            debug->info("Generate SRAM circuit successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("Create SRAM failed~");
            std::exit(12);
        }
    }

    void ScriptXTaro::saveFiles()
    {
        try
        {
            debug->info("Write spice file...");
            this->_sram->writeSpice(option->spicePath);
            debug->info("Write spice file successfully!");

            debug->info("Write verilog file...");
            parse::Verilog verilog {option->addressWidth, option->wordWidth};
            verilog.writeSRAM(option->verilogPath);
            debug->info("Write verilog file successfully!");

            debug->info("Begin to functional tests...");
            std::vector<PVT> pvts {this->pvtLists()};
            for (const PVT& pvt : pvts)
            {
                debug->info(
                    "Functional test in 'PVT(%s, %f, %f)'", 
                    pvt.process.c_str(), pvt.voltage, pvt.temperature
                );
                character::FunctionSimulator function {this->_sram.get(), pvt};
                bool result {function.randomTest()};
                if (result)
                    debug->info("Functional test pass");
                else
                    debug->info("Functional test failed");
            }
            debug->info("Functional tests successfully!");
        }
        catch (const std::exception& err)
        {
            debug->error("Save files failed~");
            std::exit(12);
        }
    }

    std::vector<PVT> ScriptXTaro::pvtLists()
    {
        std::vector<PVT> pvts {};
        for (const std::string& p : option->processCorners )
        {
            for (const double v : option->supplyVoltages)
            {
                for (const double t : option->temperatures)
                {
                    pvts.emplace_back(p, v, t);
                }
            }
        }
        return pvts;
    }
}