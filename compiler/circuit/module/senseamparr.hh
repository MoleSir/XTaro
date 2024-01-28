#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int wordWidth);
    */
    struct SenseAmplifierArrayArguments : public CircuitArguments
    {
        SenseAmplifierArrayArguments(int ww) :
            wordWidth{ww} {}

        virtual ~SenseAmplifierArrayArguments() noexcept override {}

        virtual std::string toString() const override;

        int wordWidth;
    };

    /*
        - Ports sequency: bl0 bl1 ... br0 br1 ... dout0 dout1 ... sa_en vdd gnd
    */
    class SenseAmplifierArray : public Circuit
    {
        enum 
        {
            MAX_FANOUT = 4,
        };

    public:
        SenseAmplifierArray(const std::string_view& name, SenseAmplifierArrayArguments* arguments);
        virtual ~SenseAmplifierArray() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _wordWidth;
        int _fanoutSize;

        Circuit* _senseamp;
        Circuit* _fanoutbuf;
    };

}