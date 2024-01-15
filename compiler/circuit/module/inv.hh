#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (DriveCapability dc);
    */
    struct INVArguments : public CircuitArguments
    {
        INVArguments(DriveCapability dc) : 
            driveCapability{dc} {}

        virtual ~INVArguments() noexcept override {}

        virtual std::string toString() const override;

        DriveCapability driveCapability;
    };

    /*
        - Ports sequency: A Z vdd gnd
    */
    class INV : public Circuit
    {
    public:
        INV(String name, INVArguments* arguments);
        virtual ~INV() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        DriveCapability _driveCapability;

        Circuit* _nmos;
        Circuit* _pmos;
    };

}