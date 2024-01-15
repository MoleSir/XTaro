#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct WriteDriverArguments : public CircuitArguments
    {
        WriteDriverArguments() noexcept = default;
        virtual ~WriteDriverArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: din bl br en vdd gnd
    */
    class WriteDriver : public Circuit
    {
    public:
        WriteDriver(String name, WriteDriverArguments* arguments);
        virtual ~WriteDriver() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}