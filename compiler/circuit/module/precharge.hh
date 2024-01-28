#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct PrechargeArguments : public CircuitArguments
    {
        PrechargeArguments() noexcept = default;
        virtual ~PrechargeArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: bl br en_bar vdd gnd
    */
    class Precharge : public Circuit
    {
    public:
        Precharge(const std::string_view& name, PrechargeArguments* arguments);
        virtual ~Precharge() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}