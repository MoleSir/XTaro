#pragma once

#include <config/PVT.hh>
#include <fstream>
#include <vector>

namespace xtaro
{
    namespace circuit
    {
        class SRAM;
    }

    namespace character
    {
        class CharacterResult;
    }
}

namespace xtaro::parse
{

    class Liberty 
    {
    public:
        Liberty(
            circuit::SRAM* sram,
            character::CharacterResult* results,
            PVT pvt);

    public:
        void write();

    private:
        void writeLibrary();
        void writeCell();

    private:
        void writeUnits();
        void writeDefaults();
        void writeLUTTemplates();
        void writeBus();
        void writePGPin();

        void writeWordBus();
        void writeWordBusInput();
        void writeWordBusOutput();

        void writeAdderBus();
        void writeSetupHold();
        
        void writeControlPins();
        
    private:
        void writeIndex(int number, const std::vector<double>& values);

    private:
        void writeLine(int indent, const std::string& content);
        void writeLine(int indent, const char* fmt, ...);
        void writeLine(int indent, char ch);
        void writeEntry();

    private:
        circuit::SRAM* _sram;
        character::CharacterResult* _results;
        PVT _pvt;

        std::ofstream _libfile;
    };

}