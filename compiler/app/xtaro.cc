#include "xtaro.hh"
#include <global.hh>
#include <factory/circuitfactory.hh>

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
        circuit::Circuit* c = 
            circuit::factory->create(circuit::CircuitType::BITCELL, "bitcell");
        c->writeSpice("./temp/output/sram.sp");
    }

    void saveFiles()
    {

    }
}
