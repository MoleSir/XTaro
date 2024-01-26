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
            option->load(optionFile);
            
            debug->init(option->outputPath + "xtaro.log");
            debug->info("Init XTaro.");
            debug->setLevel(DebugLevel::DEBUG);

            tech->load();
        }
        catch (const std::exception& err)
        {
            std::cout << "Init Xtaro failed! " << err.what() << std::endl;
            std::exit(12);
        }
    }

    void ScriptXTaro::createSRAM()
    {
        try
        {
            debug->info("Generate SRAM circuit.");
            circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
            this->_sram = std::make_unique<circuit::SRAM>(option->sramName, &argument);
        }
        catch (const std::exception& err)
        {
            std::cout << "Create SRAM failed! " << err.what() << std::endl;
            std::exit(12);
        }
    }

    void ScriptXTaro::saveFiles()
    {
        try
        {
            debug->info("Write spice file.");
            this->_sram->writeSpice(option->spicePath);

            debug->info("Write verilog file.");
            parse::Verilog verilog {option->addressWidth, option->wordWidth};
            verilog.writeSRAM(option->verilogPath);

            debug->info("Begin to functional test.");
            std::vector<PVT> pvts {this->pvtLists()};
            for (const PVT& pvt : pvts)
            {
                debug->info(
                    "Begin to functional test in 'PVT(%s, %f, %f)'", 
                    pvt.process.c_str(), pvt.voltage, pvt.temperature
                );
                character::FunctionSimulator function {this->_sram.get(), pvt};
                bool result {function.randomTest()};
                if (result)
                    debug->info("Functional test pass");
                else
                    debug->fatal("Functional test failed");
            }
        }
        catch (const std::exception& err)
        {
            std::cout << "Save files failed! " << err.what() << std::endl;
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