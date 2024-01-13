#include "or.hh"

#include <module/nor.hh>
#include <module/inv.hh>
#include <factory/circuitfactory.hh>

#include <allocator/allocator.hh>
#include <tech/tech.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string ORArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    OR::OR(std::string name, ORArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nor{nullptr},
        _inv{nullptr}
    {
        this->createNetlist();
    }

    void OR::createPorts()
    {
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);

        this->addPort("Z", PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void OR::createCircuits()
    {   
        NORArguments norArgument{this->_driveCapability, this->_inputSize};
        this->_nor = factory->create(ModuleType::NOR, &norArgument);
        this->_circuits.insert(this->_nor);

        INVArguments invArgument{this->_driveCapability};
        this->_inv = factory->create(ModuleType::INV, &invArgument);
        this->_circuits.insert(this->_inv);
    } 

    void OR::createInstances() 
    {
        std::vector<std::string> nets{};
        for (int i = 0; i < this->_inputSize; ++i)
            nets.emplace_back( util::format("A%d", i) );
        nets.emplace_back("Z_int");
        nets.emplace_back("vdd");
        nets.emplace_back("gnd");
        
        Instance* nor{ Allocator::alloc<Instance>("nor", this->_nor) };
        this->_instances.emplace_back(nor);
        this->connectWith(nor, std::move(nets));

        Instance* inv{ Allocator::alloc<Instance>("inv", this->_inv) };
        this->_instances.emplace_back(inv);
        this->connectWith(inv, {"Z_int", "Z", "vdd", "gnd"});
    }
}