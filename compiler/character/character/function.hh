#pragma once

#include <simulator/sramsimulator.hh>
#include <random>

namespace xtaro::character
{
    class FunctionSimulator : public SRAMSimulator
    {
        using AddrGenerator = std::uniform_int_distribution<unsigned int>;
        using WordGenerator = std::uniform_int_distribution<unsigned int>;
        using OperGenerator = std::uniform_int_distribution<unsigned int>;

        static constexpr double deviation {0.1};

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
        bool checkSimluationReuslts(const std::map<std::string, double>& results) const;

    private:
        unsigned int  randomAddress();
        unsigned int  randomWord();
        SRAMOperation randomOperation();

    private:
        std::mt19937 _engine;
        AddrGenerator _addrGenerator;
        WordGenerator _wordGenerator;
        OperGenerator _operGenerator;
    };
}