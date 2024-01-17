#include "xtaro.hh"
#include <global.hh>
#include <factory/circuitfactory.hh>
#include <module/sram.hh>
#include <verilog/verilog.hh>

namespace xtaro
{
    void init(const std::string& configfile)
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

    void createSRAM()
    {
        try
        {
            circuit::SRAMArguments argument {config->addressWidth, config->wordWidth};
            circuit::Circuit* c = 
                circuit::factory->create(circuit::ModuleType::SRAM, &argument);
            c->writeSpice("./temp/output/sram.sp");

            parse::Verilog verilog {config->addressWidth, config->wordWidth};
            verilog.writeSRAM("./temp/output/sram.v");
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            std::exit(12);
        }
    }

    void saveFiles()
    {

    }
}
