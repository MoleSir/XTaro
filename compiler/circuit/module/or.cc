#include "or.hh"

#include <module/nor.hh>
#include <module/inv.hh>
#include <factory/circuitfactory.hh>

#include <allocator/allocator.hh>
#include <config/tech.hh>
#include <util/format.hh>
#include <debug/logger.hh>
#include <exception/msgexception.hh>

namespace xtaro::circuit
{
    std::string ORArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    OR::OR(String name, ORArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nor{nullptr},
        _inv{nullptr}
    {
        if (this->_inputSize < 2)
        {
            std::string errorMsg {util::format("OR gate's input size '%d' < 2", this->_inputSize)};

            logger->error(errorMsg);
            throw MessageException("Create OR", errorMsg);
        }

        logger->debug("Create a 'OR' circuit: '%s'", this->_name.cstr());
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
        std::vector<String> nets{};
        for (int i = 0; i < this->_inputSize; ++i)
            nets.emplace_back( util::format("A%d", i) );
        nets.emplace_back("Z_bar");
        nets.emplace_back("vdd");
        nets.emplace_back("gnd");
        
        Instance* nor{ this->addInstance("nor", this->_nor) };
        this->connectWith(nor, nets);

        Instance* inv{ this->addInstance("inv", this->_inv) };
        this->connectWith(inv, {"Z_bar", "Z", "vdd", "gnd"});
    }
}