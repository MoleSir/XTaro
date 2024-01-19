#include "function.hh"

#include <command/meas.hh>

#include <util/util.hh>
#include <config/config.hh>

#include <random>

namespace xtaro::character
{

    FunctionSimulator::FunctionSimulator(
        std::string simulationFilename, circuit::SRAM* sram, PVT pvt
    ) :
        SRAMSimulator{std::move(simulationFilename), sram, std::move(pvt)},
        _engine{std::random_device{}()},
        _addrGenerator{0, util::fullBitsNumber(this->_sram->addressWidth())},
        _wordGenerator{0, util::fullBitsNumber(this->_sram->wordWidth())},
        _operGenerator{0, 1}
    {
    }

    bool FunctionSimulator::randomTest(int periods)
    {
        this->generateRandomTransactions(periods);
        // auto results {this->run(util::format("%sfunctoin.res", config->outputPath.c_str()))};
        // return this->checkSimluationReuslts(results);
        return true;
    }

    void FunctionSimulator::
    generateRandomTransactions(int periods)
    {
        for (int i = 0; i < periods; ++i)
        {
            SRAMOperation operation {this->randomOperation()};
            unsigned int address {this->randomAddress()};
            
            if (operation == SRAMOperation::WRITE || 
                this->_memoryState.find(address) == this->_memoryState.end() )
            {
                // Read a unwirte address, turn to write
                unsigned int word {this->randomWord()};
                this->addWriteTransaction(address, word);
            }
            else
                this->addReadTransaction(address);
        }

        this->writeTransactions();
    }

    bool FunctionSimulator::
    checkSimluationReuslts(const std::map<std::string, double>& results) const
    {
        for (const ReadChecker& checker : this->_readCheckers)
        {
            const Measurement* meas {checker.first};
            auto result {results.find(meas->name())};
            
            if ( util::deviation(result->second, checker.second) > FunctionSimulator::deviation )
                return false;
        }
        return true;
    }

    unsigned int FunctionSimulator::randomAddress()
    {
        return this->_addrGenerator(this->_engine);
    }
    
    unsigned int FunctionSimulator::randomWord()
    {
        return this->_wordGenerator(this->_engine);
    }

    SRAMOperation FunctionSimulator::randomOperation()
    {
        unsigned int oper {this->_wordGenerator(this->_engine)};
        return oper == 1 ? SRAMOperation::WRITE : SRAMOperation::READ;
    }
}