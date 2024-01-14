#include "senseamp.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    SenseAmplifier::SenseAmplifier(std::string name, SenseAmplifierArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->techpath + "/spice/sense_amp.sp"}
    {
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