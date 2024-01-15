#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    /*
        - Construction: (int wordWidth, int addressWidth);
    */
    struct BitcellArrayArguments : public CircuitArguments
    {
        BitcellArrayArguments(int rs, int cs) :
            rowSize{rs}, columnSize{cs} {}

        virtual ~BitcellArrayArguments() noexcept override {};

        virtual std::string toString() const override;

        int rowSize;
        int columnSize;
    };

    /*
        - Ports sequency: bl0 ... bln br0 ... brn ... wl0 ... wlm vdd gnd;
    */
    class BitcellArray : public Circuit
    {
    public:
        BitcellArray(String name, BitcellArrayArguments* arguments);
        virtual ~BitcellArray() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _rowSize;
        int _columnSize;
        
        Circuit* _bitcell;
    };

}