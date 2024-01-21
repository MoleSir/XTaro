#include "xtaro.hh"

#include <config/option.hh>
#include <config/tech.hh>
#include <debug/logger.hh>

#include <factory/circuitfactory.hh>
#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

namespace xtaro
{
        
    void XTaro::runScriptMode(const std::vector<std::string>& arguments)
    {
        try
        {
            this->init(arguments[0]);
            this->createSRAM();
            this->saveFiles();
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            std::exit(12);
        }
    }

    void XTaro::init(const std::string& optionFile)
    {
        option->load(optionFile);
        
        logger->open(option->outputPath + "xtaro.log");
        logger->info("Init xtaro.");
        logger->setLevel(Logger::Level::DEBUG);

        tech->load();
    }

    void XTaro::createSRAM()
    {
        logger->info("Generate SRAM circuit.");
        circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
        this->_sram = dynamic_cast<circuit::SRAM*>(
            circuit::factory->create(circuit::ModuleType::SRAM, &argument, option->sramName)
        );
    }

    void XTaro::saveFiles()
    {
        logger->info("Write spice file.");
        this->_sram->writeSpice(option->spicePath);

        logger->info("Write verilog file.");
        parse::Verilog verilog {option->addressWidth, option->wordWidth};
        verilog.writeSRAM(option->verilogPath);

        logger->info("Begin to functional test.");
        character::FunctionSimulator function {this->_sram, PVT{"TT", 3.3, 25}};
        std::cout << function.randomTest() << std::endl;
    }

}
