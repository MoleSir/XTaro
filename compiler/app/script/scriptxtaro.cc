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

    ScriptXtaro* ScriptXtaro::instance()
    {
        static ScriptXtaro _xtaro;
        return &_xtaro;
    }

    void ScriptXtaro::init(const std::string& optionFile)
    {
        option->load(optionFile);
        
        logger->open(option->outputPath + "xtaro.log");
        logger->info("Init xtaro.");
        logger->setLevel(LoggerLevel::DEBUG);

        tech->load();
    }

    void ScriptXtaro::createSRAM()
    {
        logger->info("Generate SRAM circuit.");
        circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
        this->_sram = std::make_unique<circuit::SRAM>(option->sramName, &argument);
    }

    void ScriptXtaro::saveFiles()
    {
        logger->info("Write spice file.");
        this->_sram->writeSpice(option->spicePath);

        logger->info("Write verilog file.");
        parse::Verilog verilog {option->addressWidth, option->wordWidth};
        verilog.writeSRAM(option->verilogPath);

        logger->info("Begin to functional test.");
        character::FunctionSimulator function {this->_sram.get(), PVT{"TT", 3.3, 25}};
        std::cout << function.randomTest() << std::endl;
    }

}