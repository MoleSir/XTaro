#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int addressWidth, int wordWidth);
    */
    struct InputDFFsArguments : public CircuitArguments
    {
        InputDFFsArguments(int aw, int ww) :
            addressWidth{aw}, wordWidth{ww} {}

        virtual ~InputDFFsArguments() noexcept override {}

        virtual std::string toString() const override;

        int addressWidth;
        int wordWidth;
    };

    /*
        - Ports sequency: 
            -- clk 
            -- csb we
            -- addr0 addr1 ...
            -- din0 din1 ...
            -- csb_r we_r
            -- addr0_r addr1_r ...
            -- din0_r din1_r ...
            -- vdd gnd
    */
    class InputDFFs : public Circuit
    {
    public:
        InputDFFs(const std::string_view& name, InputDFFsArguments* arguments);
        virtual ~InputDFFs() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _addressWidth;
        int _wordWidth;

        Circuit* _dff;
    };

}