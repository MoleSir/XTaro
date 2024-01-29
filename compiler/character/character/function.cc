#include "function.hh"

#include <command/meas.hh>
#include <command/voltageatmeas.hh>

#include <util/util.hh>
#include <config/option.hh>
#include <debug/debug.hh>

#include <random>

namespace xtaro::character
{
    FunctionSimulator::FunctionSimulator(circuit::SRAM* sram, PVT pvt) :
        _simulator{std::make_unique<SRAMSimulator>(
            util::format(
                "%sfunction_%s_%.2f_%.2f.sp", 
                option->simFolderPath.c_str(),
                pvt.process.c_str(),
                pvt.voltage,
                pvt.temperature
            ), 
            sram, 
            std::move(pvt)
        )},
        
        _readRecords{},
        _targetMeasResults{},

        _engine{std::random_device{}()},
        _addrGenerator{0, util::fullBitsNumber(sram->addressWidth())},
        _wordGenerator{0, util::fullBitsNumber(sram->wordWidth())},
        _operGenerator{0, 1}
    {
    }

    bool FunctionSimulator::randomTest(int periods)
    {
        this->generateRandomTransactions(periods);
        this->writeRandomTransactions();
        // auto results {
        //     this->_simulator->run(util::format("%s/function.res", option->outputPath.c_str()))
        // };
        // return this->checkSimluationReuslts(results);
        return true;
    }

    void FunctionSimulator::addWriteTransaction(unsigned int address, unsigned int word)
    {
        this->_simulator->addWriteTransaction(address, word);
    }

    void FunctionSimulator::addReadTransaction(unsigned int address)
    {
        this->_simulator->addReadTransaction(address);
        
        // Record
        Bits bits {this->_simulator->memory(address)};
        double measTime {this->_simulator->currentTime()};
        this->_readRecords.emplace_back(std::move(bits), measTime);
    }

    void FunctionSimulator::
    generateRandomTransactions(int periods)
    {
        for (int i = 0; i < periods; ++i)
        {
            SRAMOperation operation {this->randomOperation()};
            unsigned int address {this->randomAddress()};
            
            if (operation == SRAMOperation::WRITE || 
                !this->_simulator->isWrittenMemory(address))
            {
                // Read a unwirte address, turn to write
                unsigned int word {this->randomWord()};
                this->addWriteTransaction(address, word);
            }
            else
                this->addReadTransaction(address);
        }
    }

    void FunctionSimulator::writeRandomTransactions()
    {
        // Write transactions & .Trans
        this->_simulator->writeTransactions();

        // Write .MEAS
        for (std::size_t i = 0; i < this->_readRecords.size(); ++i)
        {
            const ReadRecord& record = this->_readRecords[i];

            // For each bit
            for (std::size_t bitIdx = 0; bitIdx < record.bits.size(); ++bitIdx)
            {
                bool bit {record.bits[bitIdx]};
                std::string measName {util::format("D%d_%d", bitIdx, i)};
                double targetResult {bit ? this->_simulator->vdd() : 0.0};

                this->_simulator->writeMeasurement(
                    std::make_unique<VoltageAtMeasurement>(
                        measName,
                        util::format("D%d", bitIdx),
                        record.measureTime
                    )
                );
                this->_targetMeasResults.emplace(std::move(measName), targetResult);
            }

            this->_simulator->writeContent('\n');
        }
    }

    bool FunctionSimulator::
    checkSimluationReuslts(const std::map<std::string, double>& results) const
    {
        for (const auto& kv : this->_targetMeasResults)
        {
            const std::string& measName {kv.first};
            auto iter {results.find(measName)};
            if (iter == results.end())
            {
                debug->error("The .MEAS task '%s' failed!", measName.c_str());
                return false;                
            }
            else
            {
                double tartgetResult {kv.second};
                double realResult {iter->second};
                if ( util::deviation(tartgetResult, realResult) > FunctionSimulator::deviation )
                    return false;
            }
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