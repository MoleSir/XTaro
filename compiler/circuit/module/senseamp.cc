#include "senseamp.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/logger.hh>

namespace xtaro::circuit
{
    SenseAmplifier::SenseAmplifier(String name, SenseAmplifierArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->senseampSpicePath}
    {
        logger->debug("Create a 'Sense Amplifier' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void SenseAmplifier::createPorts()
    {
        // bl br dout en vdd gnd
        this->addPort("bl", PortType::INOUT );
        this->addPort("br", PortType::INOUT );
        this->addPort("dout", PortType::OUTPUT );
        this->addPort("en", PortType::INPUT );
        this->addPort("vdd", PortType::INOUT );
        this->addPort("gnd", PortType::INOUT );
    }

    void SenseAmplifier::createCircuits()
    {

    } 

    void SenseAmplifier::createInstances() 
    {

    }
}