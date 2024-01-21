#include "precharge.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>
#include <debug/logger.hh>

namespace xtaro::circuit
{
    Precharge::Precharge(String name, PrechargeArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->prechargeSpicePath}
    {
        logger->debug("Create a 'Precharge' circuit: '%s'", this->_name.cstr());
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