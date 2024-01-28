#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int wordWidth);
    */
    struct WriteDriverArrayArguments : public CircuitArguments
    {
        WriteDriverArrayArguments(int ww) :
            wordWidth{ww} {}

        virtual ~WriteDriverArrayArguments() noexcept override {}

        virtual std::string toString() const override;

        int wordWidth;
    };

    /*
        - Ports sequency: bin0 bin1 ... bl0 bl1 ... br0 br1 ... we_en vdd gnd
    */
    class WriteDriverArray : public Circuit
    {
        enum 
        {
            MAX_FANOUT = 4,
        };

    public:
        WriteDriverArray(const std::string_view& name, WriteDriverArrayArguments* arguments);
        virtual ~WriteDriverArray() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _wordWidth;
        int _fanoutSize;

        Circuit* _writedriver;
        Circuit* _fanoutbuf;
    };

}