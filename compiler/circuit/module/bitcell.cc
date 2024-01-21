#include "bitcell.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>
#include <debug/logger.hh>

namespace xtaro::circuit
{
    Bitcell::Bitcell(String name, BitcellArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->bitcellSpicePath}
    {
        logger->debug("Create a 'Bitcell' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void Bitcell::createPorts()
    {
        this->addPort("bl", PortType::INOUT );
        this->addPort("br", PortType::INOUT );
        this->addPort("wl", PortType::INPUT );
        this->addPort("vdd", PortType::INOUT );
        this->addPort("gnd", PortType::INOUT );
    }

    void Bitcell::createCircuits()
    {

    } 

    void Bitcell::createInstances() 
    {

    }
}