#include "bitcell.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    Bitcell::Bitcell(std::string name, BitcellArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->techpath + "/spice/bitcell.sp"}
    {
        this->createNetlist();
    }

    void Bitcell::createPorts()
    {
        this->addPort("bl", PortType::INPUT );
        this->addPort("br", PortType::INPUT );
        this->addPort("wl", PortType::INPUT );
        this->addPort("vdd", PortType::INPUT );
        this->addPort("gnd", PortType::INPUT );
    }

    void Bitcell::createCircuits()
    {

    } 

    void Bitcell::createInstances() 
    {

    }
}