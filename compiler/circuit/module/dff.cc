#include "dff.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    DFF::DFF(String name, DFFArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->techpath + "/spice/dff.sp"}
    {
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