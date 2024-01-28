#include "bitcell.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    Bitcell::Bitcell(const std::string_view& name, BitcellArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->bitcellSpicePath}
    {
        debug->debug("Create a 'Bitcell' circuit: '%s'", this->_name.data());
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