#include "precharge.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    Precharge::Precharge(std::string name, PrechargeArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->techpath + "/spice/precharge.sp"}
    {
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