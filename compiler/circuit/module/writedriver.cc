#include "writedriver.hh"
#include <base/circuit.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    WriteDriver::WriteDriver(std::string name, WriteDriverArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT, tech->techpath + "/spice/write_driver.sp"}
    {
        this->createNetlist();
    }

    void WriteDriver::createPorts()
    {
        // din bl br en vdd gnd
        this->addPort("din", PortType::INPUT );
        this->addPort("bl", PortType::OUTPUT );
        this->addPort("br", PortType::OUTPUT );
        this->addPort("en", PortType::INPUT );
        this->addPort("vdd", PortType::INOUT );
        this->addPort("gnd", PortType::INOUT );
    }

    void WriteDriver::createCircuits()
    {

    } 

    void WriteDriver::createInstances() 
    {

    }
}