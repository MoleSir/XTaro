#include "mos.hh"
#include <base/circuit.hh>
#include <allocator/allocator.hh>
// #include <factory/circuitfactory.hh>

#include <tech/tech.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string MOSArguments::toString() const
    {
        return util::format("w%.2fu_l%.2fu", this->width, this->length);
    }

    MOS::MOS(std::string name, MOSArguments* arguments):
        Circuit{std::move(name), DeviceType::MOS},
        _width{arguments->width},
        _length{arguments->length}
    {
        this->createNetlist();
    }

    std::string MOS::argumentsList() const
    {
        return util::format("w=%.2fu l=%.2fu", this->_width, this->_length);
    }

    void MOS::createPorts()
    {
        this->addPort("S", PortType::INOUT);
        this->addPort("G", PortType::INPUT);
        this->addPort("D", PortType::INOUT);
        this->addPort("B", PortType::INOUT);
    }

    void MOS::createCircuits()
    {

    } 

    void MOS::createInstances() 
    {

    }
    
}