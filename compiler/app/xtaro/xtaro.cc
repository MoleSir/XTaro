#include "xtaro.hh"
#include <global.hh>
#include <factory/circuitfactory.hh>
#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

namespace xtaro
{
    XTaro* xTaro {XTaro::instance()};

    void XTaro::init(const std::string& configfile)
    {
        try
        {
            config->load(configfile);
            
            logger->open(config->outputPath + "xtaro.log");
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
            circuit::SRAMArguments argument {config->addressWidth, config->wordWidth};
            this->_sram = dynamic_cast<circuit::SRAM*>(
                circuit::factory->create(circuit::ModuleType::SRAM, &argument, config->sramName)
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
            this->_sram->writeSpice(config->spicePath);

            logger->info("Write verilog file.");
            parse::Verilog verilog {config->addressWidth, config->wordWidth};
            verilog.writeSRAM(config->verilogPath);

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
