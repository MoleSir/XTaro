#include "dff.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    DFF::DFF(const std::string_view& name, DFFArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->dffSpicePath}
    {
        debug->debug("Create a 'DFF' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void DFF::createPorts()
    {
        this->addPort("D", PortType::INPUT );
        this->addPort("Q", PortType::OUTPUT );
        this->addPort("clk", PortType::INPUT );
        this->addPort("vdd", PortType::INPUT );
        this->addPort("gnd", PortType::INPUT );
    }

    void DFF::createCircuits()
    {

    } 

    void DFF::createInstances() 
    {

    }
}