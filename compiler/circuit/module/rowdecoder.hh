#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int addressSize);
    */
    struct RowDecoderArguments : public CircuitArguments
    {
        RowDecoderArguments(int as) :
            addressSize{as} {}

        virtual ~RowDecoderArguments() noexcept override {}

        virtual std::string toString() const override;

        int addressSize;
    };

    /*
        - Ports sequency: wl_en A0 A1 ... An wl0 wl1 ... wl2^n-1 vdd gnd
    */
    class RowDecoder : public Circuit
    {
    public:
        RowDecoder(String name, RowDecoderArguments* arguments);
        virtual ~RowDecoder() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _addressSize;
        int _wlSize;

        Circuit* _and;
        Circuit* _decoder;
    };

}