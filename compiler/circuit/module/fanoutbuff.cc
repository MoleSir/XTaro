#include "fanoutbuff.hh"

#include <module/inv.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string FanoutBufferArguments::toString() const
    {
        return util::format("fs%d", this->fanoutSize);
    }

    FanoutBuffer::FanoutBuffer(const std::string_view& name, FanoutBufferArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _fanoutSize{arguments->fanoutSize},
        _inv{nullptr}
    {
        if (this->_fanoutSize <= 1 || this->_fanoutSize > MAX_FANOUT)
        {
            std::string errorMsg {util::format("Fanout Buffer's fanout size '%d' invalid", this->_fanoutSize)};
            debug->errorWithException("Create Fanout Buffer", errorMsg);
        }

        debug->debug("Create a 'Fanout Buffer' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void FanoutBuffer::createPorts()
    {
        this->addPort("in", PortType::INPUT);
        for (int i = 0; i < this->_fanoutSize; ++i)
            this->addPort(stringFactory->get("out%d", i), PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void FanoutBuffer::createCircuits()
    {   
        INVArguments invArugments {DriveCapability::STRONG_1};
        this->_inv = this->addCircuit("inv", &invArugments);
    } 

    void FanoutBuffer::createInstances() 
    {
        Instance* inputInv {this->addInstance("input_inv", this->_inv)};
        this->connectWith(inputInv, {"in", "in_bar", "vdd", "gnd"});
        
        for (int i = 0; i < this->_fanoutSize; ++i)
        {
            Instance* outInv {this->addInstance(stringFactory->get("inv%d", i), this->_inv)};
            this->connectWith(outInv, {
                "in_bar", stringFactory->get("out%d", i), "vdd", "gnd"
            });
        }
    }
}