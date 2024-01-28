#include "writedriver.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    WriteDriver::WriteDriver(const std::string_view& name, WriteDriverArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT, tech->writedriverSpicePath}
    {
        debug->debug("Create a 'Write Driver' circuit: '%s'", this->_name.data());
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