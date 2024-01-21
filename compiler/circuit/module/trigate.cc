#include "trigate.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>
#include <debug/logger.hh>

namespace xtaro::circuit
{
    TristateGate::TristateGate(String name, TristateGateArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->trigateSpicePath}
    {
        logger->debug("Create a 'Tristate Gate' circuit: '%s'", this->_name.cstr());
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