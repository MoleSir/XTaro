#include "and.hh"

#include <module/nand.hh>
#include <module/inv.hh>

#include <stringpool/string.hh>
#include <module/mos.hh>
#include <config/tech.hh>

#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string ANDArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    AND::AND(String name, ANDArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nand{nullptr},
        _inv{nullptr}
    {
        if (this->_inputSize < 2)
            debug->errorWithException(
                "Create AND", util::format("AND gate's input size '%d' < 2", this->_inputSize)
            );

        debug->debug("Create a 'AND' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void AND::createPorts()
    {
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);

        this->addPort("Z", PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void AND::createCircuits()
    {   
        NANDArguments nandArgument{this->_driveCapability, this->_inputSize};
        this->_nand = this->addCircuit("nand", &nandArgument);

        INVArguments invArgument{this->_driveCapability};
        this->_inv = this->addCircuit("inv", &invArgument);
    } 

    void AND::createInstances() 
    {
        std::vector<String> nets{};
        for (int i = 0; i < this->_inputSize; ++i)
            nets.emplace_back( util::format("A%d", i) );
        nets.emplace_back("Z_bar");
        nets.emplace_back("vdd");
        nets.emplace_back("gnd");
        
        Instance* nand{ this->addInstance("nand", this->_nand) };
        this->connectWith(nand, nets);

        Instance* inv{ this->addInstance("inv", this->_inv) };
        this->connectWith(inv, {"Z_bar", "Z", "vdd", "gnd"});
    }
}