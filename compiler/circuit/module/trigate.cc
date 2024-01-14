#include "trigate.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    TristateGate::TristateGate(std::string name, TristateGateArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->techpath + "/spice/tri_gate.sp"}
    {
        this->createNetlist();
    }

    void TristateGate::createPorts()
    {
        // in out en en_bar vdd gnd
        this->addPort("in", PortType::INPUT );
        this->addPort("out", PortType::OUTPUT );
        this->addPort("en", PortType::INPUT );
        this->addPort("en_bar", PortType::INPUT );
        this->addPort("vdd", PortType::INOUT );
        this->addPort("gnd", PortType::INOUT );
    }

    void TristateGate::createCircuits()
    {

    } 

    void TristateGate::createInstances() 
    {

    }
}