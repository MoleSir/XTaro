#include "xtaro.hh"
#include <global.hh>
#include <factory/circuitfactory.hh>
#include <module/bitcellarray.hh>
#include <module/mos.hh>
#include <module/inv.hh>
#include <module/nand.hh>
#include <module/and.hh>
#include <module/nor.hh>
#include <module/or.hh>
#include <module/decoder.hh>
#include <module/rowdecoder.hh>
#include <module/mux.hh>
#include <module/columnmux.hh>
#include <module/senseamparr.hh>
#include <module/writedriverarr.hh>
#include <module/prechargearr.hh>
#include <module/bank.hh>
#include <module/controllogic.hh>
#include <module/sram.hh>

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
            circuit::SRAMArguments argument{config->addressWidth, config->wordWidth};
            circuit::Circuit* c = 
                circuit::factory->create(circuit::ModuleType::SRAM, &argument);
            c->writeSpice("./temp/output/sram.sp");
                
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
