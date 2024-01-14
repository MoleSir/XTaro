#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct BitcellArguments : public CircuitArguments
    {
        BitcellArguments() noexcept = default;
        virtual ~BitcellArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: bl br wl vdd gnd;
    */
    class Bitcell : public Circuit
    {
    public:
        Bitcell(std::string name, BitcellArguments* arguments);
        virtual ~Bitcell() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}