#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (DriveCapability dc, int inputSize);
    */
    struct ORArguments : public CircuitArguments
    {
        ORArguments(DriveCapability dc, int is) : 
            driveCapability{dc}, inputSize{is} {}

        virtual ~ORArguments() noexcept override {}

        virtual std::string toString() const override;

        DriveCapability driveCapability;
        int inputSize;
    };

    /*
        - Ports sequency: A0 A1 ... An Z vdd gnd
    */
    class OR : public Circuit
    {
    public:
        OR(std::string name, ORArguments* arguments);
        virtual ~OR() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        DriveCapability _driveCapability;
        int _inputSize;

        Circuit* _nor;
        Circuit* _inv;
    };

}