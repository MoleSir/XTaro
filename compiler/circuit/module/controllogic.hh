#pragma once

#include <stringpool/string.hh>
#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: ();
    */
    struct ControlLogicArguments : public CircuitArguments
    {
        ControlLogicArguments() {}

        virtual ~ControlLogicArguments() noexcept override {}

        virtual std::string toString() const override;
    };

    /*
        - Ports sequency: clk csb we rbl wl_en p_en_bar sa_en we_en vdd gnd
    */
    class ControlLogic : public Circuit
    {
    public:
        ControlLogic(String name, ControlLogicArguments* arguments);
        virtual ~ControlLogic() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        Circuit* _inv;
        Circuit* _powerInv;
        Circuit* _or2;
        Circuit* _nor2;
        Circuit* _nor3;
        Circuit* _nand2;
    };

}