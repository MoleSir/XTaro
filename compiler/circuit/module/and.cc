#include "and.hh"

#include <module/nand.hh>
#include <module/inv.hh>

#include <factory/circuitfactory.hh>
#include <module/mos.hh>
#include <tech/tech.hh>

#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string ANDArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    AND::AND(std::string name, ANDArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nand{nullptr},
        _inv{nullptr}
    {
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
        this->_nand = factory->create(ModuleType::NAND, &nandArgument);
        this->_circuits.insert(this->_nand);

        INVArguments invArgument{this->_driveCapability};
        this->_inv = factory->create(ModuleType::INV, &invArgument);
        this->_circuits.insert(this->_inv);
    } 

    void AND::createInstances() 
    {
        std::vector<std::string> nets{};
        for (int i = 0; i < this->_inputSize; ++i)
            nets.emplace_back( util::format("A%d", i) );
        nets.emplace_back("Z_bar");
        nets.emplace_back("vdd");
        nets.emplace_back("gnd");
        

        Instance* nand{ this->addInstance("nand", this->_nand) };
        this->connectWith(nand, std::move(nets));

        Instance* inv{ this->addInstance("inv", this->_inv) };
        this->connectWith(inv, {"Z_bar", "Z", "vdd", "gnd"});
    }
}