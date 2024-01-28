#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int fanoutSize);
    */
    struct FanoutBufferArguments : public CircuitArguments
    {
        FanoutBufferArguments(int fs) :
            fanoutSize{fs} {}

        virtual ~FanoutBufferArguments() noexcept override {}

        virtual std::string toString() const override;

        int fanoutSize;
    };

    /*
        - Ports sequency: in out0 out1 ... vdd gnd
    */
    class FanoutBuffer : public Circuit
    {
        enum 
        {
            MAX_FANOUT = 8,
        };

    public:
        FanoutBuffer(const std::string_view& name, FanoutBufferArguments* arguments);
        virtual ~FanoutBuffer() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _fanoutSize;

        Circuit* _inv;
    };

}