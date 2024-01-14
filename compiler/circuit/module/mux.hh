#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int selectionSize);
    */
    struct MuxArguments : public CircuitArguments
    {
        MuxArguments(int ss) :
            selectionSize{ss} {}

        virtual ~MuxArguments() noexcept override {}

        virtual std::string toString() const override;

        int selectionSize;
    };

    /*
        - Ports sequency: S0 S1 ... Sn-1 bl0 bl1 ... bl2^n-1 br0 br1 ... br2^n-1 bl br vdd gnd 
    */
    class Mux : public Circuit
    {
    public:
        enum 
        {
            MAX_SELECTION_SIZE = 4,
        };

    public:
        Mux(std::string name, MuxArguments* arguments);
        virtual ~Mux() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _selectionSize;
        int _inputSize;

        Circuit* _decoder;
        Circuit* _trigate;
        Circuit* _inv;
    };

}