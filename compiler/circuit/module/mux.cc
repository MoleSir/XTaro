#include "mux.hh"

#include <module/decoder.hh>
#include <module/inv.hh>
#include <module/trigate.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string MuxArguments::toString() const
    {
        return util::format("ss%d", this->selectionSize);
    }

    Mux::Mux(std::string name, MuxArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _selectionSize{arguments->selectionSize},
        _inputSize{util::power(2, arguments->selectionSize)},
        _decoder{nullptr},
        _trigate{nullptr},
        _inv{nullptr}
    {
        this->createNetlist();
    }

    void Mux::createPorts()
    {
        for (int i = 0; i < this->_selectionSize; ++i)
            this->addPort(util::format("S%d", i), PortType::INPUT);

        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("bl%d", i), PortType::INOUT);
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("br%d", i), PortType::INOUT);

        this->addPort("bl", PortType::INOUT);
        this->addPort("br", PortType::INOUT);
    
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void Mux::createCircuits()
    {   
    } 

    void Mux::createInstances() 
    {
    }
}