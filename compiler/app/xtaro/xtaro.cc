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
    XTaro* xTaro {XTaro::instance()};

    void XTaro::init(const std::string& optionFile)
    {
        try
        {
            option->load(optionFile);
            
            logger->open(option->outputPath + "xtaro.log");
            logger->info("Init xtaro.");
            logger->setLevel(Logger::Level::DEBUG);

            tech->load();
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            logger->fatal(err.what());
            std::exit(12);
        }
    }

    void XTaro::createSRAM()
    {
        try
        {
            logger->info("Generate SRAM circuit.");
            circuit::SRAMArguments argument {option->addressWidth, option->wordWidth};
            this->_sram = dynamic_cast<circuit::SRAM*>(
                circuit::factory->create(circuit::ModuleType::SRAM, &argument, option->sramName)
            );
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            std::exit(12);
        }
    }

    void XTaro::saveFiles()
    {
        try
        {
            logger->info("Write spice file.");
            this->_sram->writeSpice(option->spicePath);

            logger->info("Write verilog file.");
            parse::Verilog verilog {option->addressWidth, option->wordWidth};
            verilog.writeSRAM(option->verilogPath);

            logger->info("Begin to functional test.");
            character::FunctionSimulator function {this->_sram, PVT{"TT", 3.3, 25}};
            function.randomTest();
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            std::exit(12);
        }
    }

    XTaro* XTaro::instance()
    {
        static XTaro _xTaro{};
        return &_xTaro;
    }
}
