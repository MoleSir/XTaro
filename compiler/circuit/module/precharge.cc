#include "precharge.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    Precharge::Precharge(const std::string_view& name, PrechargeArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->prechargeSpicePath}
    {
        debug->debug("Create a 'Precharge' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void Precharge::createPorts()
    {
        // bl br en_bar vdd gnd
        this->addPort("bl", PortType::INOUT );
        this->addPort("br", PortType::INOUT );
        this->addPort("en_bar", PortType::INPUT );
        this->addPort("vdd", PortType::INOUT );
        this->addPort("gnd", PortType::INOUT );
    }

    void Precharge::createCircuits()
    {

    } 

    void Precharge::createInstances() 
    {

    }
}