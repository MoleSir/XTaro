#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{
    struct TristateGateArguments : public CircuitArguments
    {
        TristateGateArguments() noexcept = default;
        virtual ~TristateGateArguments() noexcept override {}
        virtual std::string toString() const override 
        { return ""; }
    };

    /*
        - Ports sequency: in out en en_bar vdd gnd
    */
    class TristateGate : public Circuit
    {
    public:
        TristateGate(const std::string_view& name, TristateGateArguments* arguments);
        virtual ~TristateGate() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;
    };

}