#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct BitcellArrayArguments : public CircuitArguments
    {
        BitcellArrayArguments(std::int32_t w, std::int32_t a) :
            wordWidth{w}, addressWidth{a} {}
        std::int32_t wordWidth;
        std::int32_t addressWidth;
    };

    /*
        - Ports sequency: bl0 ... bln br0 ... brn ... wl0 ... wlm vdd gnd;
        - Construction: (std::string name, std::int32_t wordWidth, std::int32_t addressWidth);
    */
    class BitcellArray : public Circuit
    {
    public:
        BitcellArray(std::string name, BitcellArrayArguments* arguments);
        virtual ~BitcellArray() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        std::int32_t _wordWidth;
        std::int32_t _addressWidth;
        std::int32_t _rowSize;
        std::int32_t _columnSize;
        Circuit* _bitcell;
    };

}