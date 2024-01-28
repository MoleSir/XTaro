#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct DFFArguments : public CircuitArguments
    {
        DFFArguments() noexcept = default;
        virtual ~DFFArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: D Q clk vdd gnd
    */
    class DFF : public Circuit
    {
    public:
        DFF(const std::string_view& name, DFFArguments* arguments);
        virtual ~DFF() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}