#pragma once

#include <base/circuit.hh>

namespace xtaro::circuit
{

    class Bitcell : public Circuit
    {
    public:
        Bitcell(std::string name);

    private:
        virtual void createPorts() override {}
        virtual void createCircuits() override {}
        virtual void createInstances() override {}
    };

}