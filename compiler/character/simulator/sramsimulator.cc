#include "sramsimulator.hh"
#include <config/tech.hh>
#include <config/option.hh>
#include <command/voltageatmeas.hh>

#include <util/format.hh>
#include <util/math.hh>
#include <debug/logger.hh>

namespace xtaro::character
{

    SRAMSimulator::SRAMSimulator(
        std::string simulationFilename, circuit::SRAM* sram, PVT pvt
    ) :
        Simulator{simulationFilename},
        
        _sram{sram},
        
        _pvt{std::move(pvt)},
        _period{tech->spice["period"].asInteger()},
        // TODO slew
        _slew{0.4},
        
        _addrMask{util::fullBitsNumber(sram->addressWidth())},
        _wordMask{util::fullBitsNumber(sram->wordWidth())},

        _transactions{},
        _memoryState{}
    {
        // Initial base spice command
        // Include command
        std::string nmosModelPath {
            option->techPath + tech->spice["models"][this->_pvt.process][0].asString()
        };

        std::string pmosModelPath {
            option->techPath + tech->spice["models"][this->_pvt.process][0].asString()
        };

        this->writeInclude(nmosModelPath);
        this->writeInclude(pmosModelPath);
        this->writeInclude(option->spicePath);
        this->writeContent('\n');

        // Voltage & Temperature
        this->writeDCVoltage("vdd", "vdd", this->_pvt.temperature);
        this->writeTemperature(this->_pvt.temperature);
        this->writeContent('\n');

        // Instantiation of the SRAM
        std::vector<std::string> nets {this->_sram->portsName()};
        this->writeInstance(
            this->_sram->name().cstr(),
            "sram",
            nets
        );
        this->writeContent('\n');
    
        // Write CLK
        this->writePULSEVoltage(
            "clk", "clk", 
            0.0, this->_pvt.voltage,
            0.0, this->_slew, this->_slew,
            this->_period / 2, this->_period
        );
        this->writeContent('\n');
    }

    bool SRAMSimulator::addWriteTransaction(unsigned int address, unsigned int word)
    {
        address = this->resetAddress(address);
        word = this->resetWord(word);

        // Add transcation
        this->_transactions.emplace_back(
            SRAMOperation::WRITE,
            this->addressBits(address),
            this->wordBits(word)
        );

        // Record memory state
        this->_memoryState[address] = this->wordBits(word);
        this->writeComment(this->writeTransactionComment(address, word));

        return true;
    }

    bool SRAMSimulator::addReadTransaction(unsigned int address)
    {
        address = this->resetAddress(address);

        auto iter {this->_memoryState.find(address)};
        if (iter == this->_memoryState.end())
        {   
            logger->warning("Try to read an unset address 0x%x, this transaction will be ignored.");
            return false;
        }

        // Add transcation
        this->_transactions.emplace_back(
            SRAMOperation::READ,
            this->addressBits(address),
            Bits {}
        );

        this->writeComment(this->readTransactionComment(address));
        return true;
    }

    double SRAMSimulator::currentTime() const
    {
        return (this->_transactions.size() + 1.5) * this->_period;
    }

    void SRAMSimulator::writeTransactions()
    {
        this->checkFileWritable();
        this->writeContent('\n');

        double period {static_cast<double>(this->_period)};
        double vdd {this->_pvt.voltage};

        // First period is idle
        std::vector<double> times {0.0};
        std::vector<double> csb {vdd};
        std::vector<double> we {0.0};

        std::vector<std::vector<double>> address {
            static_cast<std::size_t>(this->_sram->addressWidth()),
            std::vector<double> {0.0}
        };
        
        std::vector<std::vector<double>> word {
            static_cast<std::size_t>(this->_sram->wordWidth()),
            std::vector<double> {0.0}
        };

        // Generate each PWL
        for (std::size_t i = 0; i < this->_transactions.size(); ++i)
        {
            const SRAMTransaction& transaction {this->_transactions[i]};
            bool isWrite {transaction.operation == SRAMOperation::WRITE};

            // Times and csb increase
            times.emplace_back( period * (i + 1) );
            csb.emplace_back(0.0);

            // Write or Read
            we.emplace_back( isWrite ? vdd : 0.0);

            // Address
            for (std::size_t i = 0; i < transaction.address.size(); ++i)
                address[i].emplace_back( transaction.address[i] ? vdd : 0.0 );
            
            // Word
            if (isWrite)
            {
                for (std::size_t i = 0; i < transaction.word.size(); ++i)
                    word[i].emplace_back( transaction.word[i] ? vdd : 0.0 );
            }
            else
            {
                for (std::vector<double>& wordBit : word)
                    wordBit.emplace_back(0.0);
            }
        }

        // Write PWL
        this->writePWLVoltage("csb", "csb", times, csb, this->_slew);
        this->writePWLVoltage("we", "we", times, we, this->_slew);
        this->writeContent('\n');
        
        for (int i = 0; i < this->_sram->addressWidth(); ++i)
            this->writePWLVoltage(
                util::format("A%d", i), util::format("A%d", i),
                times, address[i], this->_slew
            );
        this->writeContent('\n');

        for (int i = 0; i < this->_sram->wordWidth(); ++i)
            this->writePWLVoltage(
                util::format("D%d", i), util::format("D%d", i),
                times, word[i], this->_slew
            );
        this->writeContent('\n');
    
        // Write .TRAN
        this->writeTrans(10, 0, period * (this->_transactions.size() + 1) );
        this->writeContent('\n');
    }

    const Bits& SRAMSimulator::memory(unsigned int address) const
    {
        return this->_memoryState.find(address)->second;
    }

    bool SRAMSimulator::isWrittenMemory(unsigned int address) const
    {
        return this->_memoryState.find(address) != this->_memoryState.end();
    }

    unsigned int SRAMSimulator::resetAddress(unsigned int address) const
    {
        unsigned int newAddr {this->_addrMask & address};
        if (newAddr != address)
            logger->warning("The address 0x%x out of range(0x%x)", address, this->_addrMask);
        return newAddr;
    }
    
    unsigned int SRAMSimulator::resetWord(unsigned int word) const
    {
        unsigned int newWord {this->_wordMask & word};
        if (newWord != word)
            logger->warning("The word 0x%x out of range(0x%x)", word, this->_wordMask);
        return newWord;
    }

    std::string SRAMSimulator::
    writeTransactionComment(unsigned int address, unsigned int word) const
    {
        int cycle {static_cast<int>(this->_transactions.size())};
        return util::format(
            "Writing 0x%x to address 0x%x during cycle %d (%dns - %dns)",
            word, address,
            cycle,
            cycle * this->_period + this->_period / 2, 
            (cycle + 1) * this->_period + this->_period / 2
        );
    }

    std::string SRAMSimulator::
    readTransactionComment(unsigned int address) const
    {
        int cycle {static_cast<int>(this->_transactions.size())};
        return util::format(
            "Reading address 0x%x during cycle %d (%dns - %dns)",
            address,
            cycle, 
            cycle * this->_period + this->_period / 2, 
            (cycle + 1) * this->_period + this->_period / 2
        );
    }

    Bits SRAMSimulator::addressBits(unsigned int address) const
    {
        return SRAMSimulator::intToBits(address, this->_sram->addressWidth());
    }
    
    Bits SRAMSimulator::wordBits(unsigned int word) const
    {
        return SRAMSimulator::intToBits(word, this->_sram->wordWidth());
    }

    Bits SRAMSimulator::intToBits(unsigned int value, int size)
    {
        // Can't use {} to init... emm...
        Bits bits(size, false);

        for (int i = 0; i < size; ++i)
        {
            bool bit {0 != (value & 0x00000001U)};
            bits[i] = bit;
            value >>= 1;
        }

        return bits;
    }
}