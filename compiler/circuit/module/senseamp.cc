#include "senseamp.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    SenseAmplifier::SenseAmplifier(const std::string_view& name, SenseAmplifierArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->senseampSpicePath}
    {
        debug->debug("Create a 'Sense Amplifier' circuit: '%s'", this->_name.data());
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