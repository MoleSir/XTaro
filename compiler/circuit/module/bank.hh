#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: ();
    */
    struct XXXArguments : public CircuitArguments
    {
        XXXArguments() {}

        virtual ~XXXArguments() noexcept override {}

        virtual std::string toString() const override;
    };

    /*
        - Ports sequency: 
    */
    class XXX : public Circuit
    {
    public:
        XXX(std::string name, XXXArguments* arguments);
        virtual ~XXX() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
    };

}