#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int addressWidth, int wordWidth);
    */
    struct BankArguments : public CircuitArguments
    {
        BankArguments(int aw, int ww) :
            addressWidth{aw}, wordWidth{ww} {}

        virtual ~BankArguments() noexcept override {}

        virtual std::string toString() const override;

        int addressWidth;
        int wordWidth;
    };

    /*
        - Ports sequency: 
            -- addr0 addr1 ... 
            -- din0 din1 ...
            -- wl_en p_en_bar sa_en we_en
            -- rbl
            -- dout0 dout1 ...
            -- vdd gnd
    */
    class Bank : public Circuit
    {
    public:
        Bank(const std::string_view& name, BankArguments* arguments);
        virtual ~Bank() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _addressWidth;
        int _wordWidth;

        int _rowSize;
        int _columnSize;
        int _rowAddressWidth;
        int _columnAddressWidth;

        Circuit* _bitcellArray;
        Circuit* _rowDecoder;
        Circuit* _columnMux;

        Circuit* _replicaBank;

        Circuit* _prechargeArray;
        Circuit* _writedriverArray;
        Circuit* _senseampArray;
    };

}