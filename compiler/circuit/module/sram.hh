#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int addressWidth, int wordWidth);
    */
    struct SRAMArguments : public CircuitArguments
    {
        SRAMArguments(int aw, int ww) :
            addressWidth{aw}, wordWidth{ww} {}

        virtual ~SRAMArguments() noexcept override {}

        virtual std::string toString() const override;

        int addressWidth;
        int wordWidth;
    };

    /*
        - Ports sequency: 
            -- clk csb we
            -- addr0 addr1 ...
            -- din0 din1 ...
            -- dout0 dout1 ...
            -- vdd gnd
    */
    class SRAM : public Circuit
    {
    public:
        SRAM(const std::string_view& name, SRAMArguments* arguments);
        virtual ~SRAM() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    public:
        int addressWidth() const noexcept
        { return this->_addressWidth; }

        int wordWidth() const noexcept
        { return this->_wordWidth; }

    private:
        int _addressWidth;
        int _wordWidth;

        Circuit* _inputDffs;
        Circuit* _bank;
        Circuit* _controllogic;
    };

}