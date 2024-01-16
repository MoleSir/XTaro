#pragma once

#include <stringpool/string.hh>
#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (int bitcellSize);
    */
    struct ReplicaBankArguments : public CircuitArguments
    {
        ReplicaBankArguments(int bs) :
            bitcellSize{bs} {}

        virtual ~ReplicaBankArguments() noexcept override {}

        virtual std::string toString() const override;

        int bitcellSize;
    };

    /*
        - Ports sequency: wl_en p_en_bar sa_en we_en rbl vdd gnd
    */
    class ReplicaBank : public Circuit
    {
        enum
        {
            LINKED_BITCELL_SIZE = 3,
        };

    public:
        ReplicaBank(String name, ReplicaBankArguments* arguments);
        virtual ~ReplicaBank() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int _bitcellSize;

        Circuit* _bitcell;
        Circuit* _prechage;
        Circuit* _writedriver;
    };

}