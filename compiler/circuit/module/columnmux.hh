#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int selectionSize, int muxSize);
    */
    struct ColumnMuxArguments : public CircuitArguments
    {
        ColumnMuxArguments(int ss, int ms) :
            selectionSize{ss}, muxSize{ms} {}

        virtual ~ColumnMuxArguments() noexcept override {}

        virtual std::string toString() const override;

        int selectionSize;
        int muxSize;
    };

    /*
        - Ports sequency: 
            -- A0 A1 ... An-1 
            -- bl0_0 bl0_1 ... 
            -- br0_0 br0_1 ...
            -- bl1_0 bl1_1 ...
            -- br1_0 br1_1 ...
            -- ...
            -- blm-1_0 blm-1_1 ...
            -- brm-1_0 brm-1_1 ...
            -- bl0 bl1 ...
            -- br0 br1 ...
            -- vdd gnd
    */
    class ColumnMux : public Circuit
    {
    public:
        ColumnMux(String name, ColumnMuxArguments* arguments);
        virtual ~ColumnMux() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _selectionSize;
        int _inputSize;
        int _muxSize;

        Circuit* _mux;
    };

}