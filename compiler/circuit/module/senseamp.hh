#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct SenseAmplifierArguments : public CircuitArguments
    {
        SenseAmplifierArguments() noexcept = default;
        virtual ~SenseAmplifierArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: bl br dout en vdd gnd
    */
    class SenseAmplifier : public Circuit
    {
    public:
        SenseAmplifier(std::string name, SenseAmplifierArguments* arguments);
        virtual ~SenseAmplifier() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}