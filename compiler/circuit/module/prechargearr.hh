#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int wordWidth);
    */
    struct PrechargeArrayArguments : public CircuitArguments
    {
        PrechargeArrayArguments(int ww) :
            wordWidth{ww} {}

        virtual ~PrechargeArrayArguments() noexcept override {}

        virtual std::string toString() const override;

        int wordWidth;
    };

    /*
        - Ports sequency: bl0 bl1 ... br0 br1 ... p_en_bar vdd gnd
    */
    class PrechargeArray : public Circuit
    {
        enum 
        {
            MAX_FANOUT = 4,
        };

    public:
        PrechargeArray(const std::string_view& name, PrechargeArrayArguments* arguments);
        virtual ~PrechargeArray() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _wordWidth;
        int _fanoutSize;

        Circuit* _precharge;
        Circuit* _fanoutbuf;
    };

}