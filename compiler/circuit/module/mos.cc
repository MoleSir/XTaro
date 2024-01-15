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
        return util::format("t%d_w%.2f_l%.2f", 
                            static_cast<int>(this->type), 
                            this->width, 
                            this->length);
    }

    MOS::MOS(String name, MOSArguments* arguments):
        Circuit{"", DeviceType::MOS},
        _type{arguments->type},
        _width{arguments->width},
        _length{arguments->length}
    {
        if (this->_type == MOSType::NMOS)
            this->_name = tech->spice["nmos"].asString();
        else
            this->_name = tech->spice["pmos"].asString();
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