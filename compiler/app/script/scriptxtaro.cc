#include "scriptxtaro.hh"

#include <config/option.hh>
#include <config/tech.hh>
#include <debug/logger.hh>

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
            
            logger->open(option->outputPath + "xtaro.log");
            logger->info("Init xtaro.");
            logger->setLevel(LoggerLevel::DEBUG);

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
            logger->info("Generate SRAM circuit.");
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
            logger->info("Write spice file.");
            this->_sram->writeSpice(option->spicePath);

            logger->info("Write verilog file.");
            parse::Verilog verilog {option->addressWidth, option->wordWidth};
            verilog.writeSRAM(option->verilogPath);

            logger->info("Begin to functional test.");
            std::vector<PVT> pvts {this->pvtLists()};
            for (const PVT& pvt : pvts)
            {
                logger->info(
                    "Begin to functional test in 'PVT(%s, %f, %f)'", 
                    pvt.process.c_str(), pvt.voltage, pvt.temperature
                );
                character::FunctionSimulator function {this->_sram.get(), pvt};
                bool result {function.randomTest()};
                if (result)
                    logger->info("Functional test pass");
                else
                    logger->error("Functional test failed");
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