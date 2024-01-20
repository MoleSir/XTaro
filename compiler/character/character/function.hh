#pragma once

#include <simulator/sramsimulator.hh>
#include <random>
#include <map>

namespace xtaro::character
{
    class FunctionSimulator
    {
        using AddrGenerator = std::uniform_int_distribution<unsigned int>;
        using WordGenerator = std::uniform_int_distribution<unsigned int>;
        using OperGenerator = std::uniform_int_distribution<unsigned int>;

        static constexpr double deviation {0.1};

        struct ReadRecord
        {  
            ReadRecord(Bits b, double mt) :
                bits{std::move(b)}, measureTime{mt} {}
            Bits bits; 
            double measureTime;
        };

    public:
        FunctionSimulator(
            std::string simulationFilename,
            circuit::SRAM* sram, 
            PVT pvt
        );
    
    public:
        bool randomTest(int periods = 20);

    private:
        void generateRandomTransactions(int periods);
        void writeRandomTransactions();
        bool checkSimluationReuslts(const std::map<std::string, double>& results) const;

        void addWriteTransaction(unsigned int address, unsigned int word);
        void addReadTransaction(unsigned int address);

    private:
        unsigned int  randomAddress();
        unsigned int  randomWord();
        SRAMOperation randomOperation();

    private:
        std::unique_ptr<SRAMSimulator> _simulator;
        std::vector<ReadRecord> _readRecords;
        std::map<std::string, double> _targetMeasResults;

        std::mt19937 _engine;
        AddrGenerator _addrGenerator;
        WordGenerator _wordGenerator;
        OperGenerator _operGenerator;
    };
}