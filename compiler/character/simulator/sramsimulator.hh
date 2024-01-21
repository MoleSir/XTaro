#pragma once

#include "simulator.hh"

#include <module/sram.hh>
#include <config/PVT.hh>

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
        bool addWriteTransaction(unsigned int address, unsigned int word);
        bool addReadTransaction(unsigned int address);
        void writeTransactions();

        const Bits& memory(unsigned int address) const;
        bool isWrittenMemory(unsigned int address) const;

        double currentTime() const;

    private:
        unsigned int resetAddress(unsigned int address) const;
        unsigned int resetWord(unsigned int word) const;

        Bits addressBits(unsigned int address) const;
        Bits wordBits(unsigned int word) const;

        std::string writeTransactionComment(unsigned int address, unsigned int word) const;
        std::string readTransactionComment(unsigned int address) const;

    protected:
        static Bits intToBits(unsigned int value, int size);

    public:
        circuit::SRAM* sram() const noexcept
        { return this->_sram; }

        double vdd() const noexcept
        { return this->_pvt.voltage; }

        const PVT& pvt() const noexcept
        { return this->_pvt; }

        const std::map<unsigned int, Bits>& memoryState() const noexcept
        { return this->_memoryState; }

    protected:
        circuit::SRAM* _sram;
        
        PVT _pvt;
        int _period;
        double _slew;

        unsigned int _addrMask;
        unsigned int _wordMask;

        std::vector<SRAMTransaction> _transactions;
        std::map<unsigned int, Bits> _memoryState;
    };

}