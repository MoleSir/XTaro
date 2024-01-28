#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (DriveCapability dc, int inputSize);
    */
    struct ANDArguments : public CircuitArguments
    {
        ANDArguments(DriveCapability dc, int is) : 
            driveCapability{dc}, inputSize{is} {}

        virtual ~ANDArguments() noexcept override {}

        virtual std::string toString() const override;

        DriveCapability driveCapability;
        int inputSize;
    };

    /*
        - Ports sequency: A0 A1 ... An Z vdd gnd
    */
    class AND : public Circuit
    {
    public:
        AND(const std::string_view& name, ANDArguments* arguments);
        virtual ~AND() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        DriveCapability _driveCapability;
        int _inputSize;

        Circuit* _nand;
        Circuit* _inv;
    };

}