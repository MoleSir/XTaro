#pragma once

#include "simulator.hh"

#include <module/sram.hh>
#include <tech/PVT.hh>

namespace xtaro::character
{
    enum class SRAMOperation
    {
        WRITE,
        READ,
    };

    using Bits = std::vector<bool>;

    struct SRAMTransaction
    {
        SRAMTransaction(SRAMOperation op, Bits a, Bits w) : 
            operation{op}, 
            address{std::move(a)}, 
            word{std::move(w)} {}

        SRAMOperation operation;
        Bits address;
        Bits word;
    };

    class SRAMSimulator : public Simulator
    {
    public:
        SRAMSimulator(
            std::string simulationFilename,
            circuit::SRAM* sram, 
            PVT pvt
        );

    public:
        void addWriteTransaction(unsigned int address, unsigned int word);
        void addReadTransaction(unsigned int address);

        void writeTransactions();

    private:
        void checkAddress(unsigned int address) const;
        void checkWord(unsigned int word) const;

        Bits addressBits(unsigned int address) const;
        Bits wordBits(unsigned int word) const;

    private:
        static Bits intToBits(unsigned int value, int size);

    protected:
        circuit::SRAM* _sram;
        PVT _pvt;
        int _period;
        double _slew;

        std::vector<SRAMTransaction> _transactions;
    };

}