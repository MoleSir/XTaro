#include "or.hh"

#include <module/nor.hh>
#include <module/inv.hh>

#include <factory/stringfactory.hh>

#include <config/tech.hh>
#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string ORArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    OR::OR(const std::string_view& name, ORArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nor{nullptr},
        _inv{nullptr}
    {
        if (this->_inputSize < 2)
        {
            std::string errorMsg {util::format("OR gate's input size '%d' < 2", this->_inputSize)};
            debug->errorWithException("Create OR", errorMsg);
        }

        debug->debug("Create a 'OR' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void OR::createPorts()
    {
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(stringFactory->get("A%d", i), PortType::INPUT);

        this->addPort("Z", PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void OR::createCircuits()
    {   
        NORArguments norArgument{this->_driveCapability, this->_inputSize};
        this->_nor = this->addCircuit("nor", &norArgument);

        INVArguments invArgument{this->_driveCapability};
        this->_inv = this->addCircuit("inv", &invArgument);
    } 

    void OR::createInstances() 
    {
        std::vector<std::string_view> nets{};
        for (int i = 0; i < this->_inputSize; ++i)
            nets.emplace_back( stringFactory->get("A%d", i) );
        nets.emplace_back("Z_bar");
        nets.emplace_back("vdd");
        nets.emplace_back("gnd");
        
        Instance* nor{ this->addInstance("nor", this->_nor) };
        this->connectWith(nor, nets);

        Instance* inv{ this->addInstance("inv", this->_inv) };
        this->connectWith(inv, {"Z_bar", "Z", "vdd", "gnd"});
    }
}