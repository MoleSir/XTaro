#pragma once

#include <string>
#include <fstream>

namespace xtaro::parse
{

    class Verilog
    {
    public:
        Verilog(int addressWidth, int wordWidth);

        void writeSRAM(const std::string& filename);

    private:
        void writeSRAMHeader(std::ofstream& file);
        void writeSRAMSignal(std::ofstream& file);
        void writeSRAMBody(std::ofstream& file);
        void writeSRAMTail(std::ofstream& file);

    private:
        int _addressWidth;
        int _wordWidth;  
    };

}