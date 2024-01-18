#include "xtaro.hh"
#include <global.hh>
#include <factory/circuitfactory.hh>
#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <simulator/sramsimulator.hh>

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
                circuit::factory->create(circuit::ModuleType::SRAM, &argument, config->sramName);
            c->writeSpice(util::format("%s/%s.sp", config->outputPath.c_str(), config->sramName.c_str()));

            parse::Verilog verilog {config->addressWidth, config->wordWidth};
            verilog.writeSRAM(util::format("./temp/output/%s.v", config->sramName.c_str()));

            character::SRAMSimulator simulator {
                "./temp/output/stim.sp", 
                dynamic_cast<circuit::SRAM*>(c), 
                PVT{"TT", 3.3, 25}
            };

            simulator.addWriteTransaction(1, 0);
            simulator.addWriteTransaction(2, 255);
            simulator.addReadTransaction(1);

            simulator.writeTransactions();
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
