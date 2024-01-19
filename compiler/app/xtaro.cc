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
            tech->load();
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
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
            this->_sram->writeSpice(
                util::format("%s/%s.sp", config->outputPath.c_str(), config->sramName.c_str())
            );

            logger->info("Write verilog file.");
            parse::Verilog verilog {config->addressWidth, config->wordWidth};
            verilog.writeSRAM(
                util::format("%s/%s.v", config->outputPath.c_str(), config->sramName.c_str())
            );

            logger->info("Begin to functional test.");
            character::FunctionSimulator function {
                util::format("%s/function.sp", config->outputPath.c_str()),
                this->_sram, 
                PVT{"TT", 3.3, 25}
            };
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
