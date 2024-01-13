#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (DriveCapability dc, int inputSize);
    */
    struct NORArguments : public CircuitArguments
    {
        NORArguments(DriveCapability dc, int is) : 
            driveCapability{dc}, inputSize{is} {}

        virtual ~NORArguments() noexcept override {}

        virtual std::string toString() const override;

        DriveCapability driveCapability;
        int inputSize;
    };

    /*
        - Ports sequency: A0 A1 ... An Z vdd gnd
    */
    class NOR : public Circuit
    {
    public:
        NOR(std::string name, NORArguments* arguments);
        virtual ~NOR() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        DriveCapability _driveCapability;
        int _inputSize;

        Circuit* _nmos;
        Circuit* _pmos;
    };

}