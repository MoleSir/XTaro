#include "prechargearr.hh"

#include <module/precharge.hh>
#include <module/fanoutbuff.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string PrechargeArrayArguments::toString() const
    {
        return util::format("ww%d", this->wordWidth);
    }

    PrechargeArray::PrechargeArray(String name, PrechargeArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _wordWidth{arguments->wordWidth},
        _precharge{nullptr},
        _fanoutSize{0},
        _fanoutbuf{nullptr}
    {
        if (this->_wordWidth < 1)
        {
            std::string errorMsg {util::format("Precharge Array's word width '%d' < 1", this->_wordWidth)};
            debug->fatal("Create Precharge Array", errorMsg);
        }

        this->_fanoutSize = this->_wordWidth / MAX_FANOUT;
        if (this->_wordWidth % MAX_FANOUT != 0) this->_fanoutSize += 1;
        
        debug->debug("Create a 'Precharge Array' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void PrechargeArray::createPorts()
    {
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("bl%d", i), PortType::INOUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("br%d", i), PortType::INOUT);
        
        this->addPort("p_en_bar", PortType::INPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void PrechargeArray::createCircuits()
    {
        this->_precharge = factory->create(CircuitType::PRECHARGE, nullptr);
        this->_circuits.emplace(this->_precharge);
        
        // If word width is too much, 'p_en_bar''s fan-out too much...
        if (this->_fanoutSize > 1)
        {
            FanoutBufferArguments fanoutArguments {this->_fanoutSize};
            this->_fanoutbuf = factory->create(CircuitType::FANOUT_BUFFER, &fanoutArguments);
            this->_circuits.emplace(this->_fanoutbuf);
        }
    } 

    void PrechargeArray::createInstances() 
    {
        if (this->_fanoutSize > 1)
        {
            Instance* fanoutbuff {this->addInstance("fanout_buff", this->_fanoutbuf)};

            std::vector<String> ports {};
            ports.emplace_back("p_en_bar");
            for (int i = 0; i < this->_fanoutSize; ++i)
                ports.emplace_back(util::format("p_en_bar%d", i));
            ports.emplace_back("vdd");
            ports.emplace_back("gnd");

            this->connectWith(fanoutbuff, ports);
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            Instance* writedriver {
                this->addInstance(util::format("precharge%d", i), this->_precharge)
            };

            this->connectWith(writedriver, {
                util::format("bl%d", i), 
                util::format("br%d", i), 
                this->_fanoutSize > 1 ? util::format("p_en_bar%d", i / MAX_FANOUT) : "p_en_bar",
                "vdd", "gnd"
            });
        }
    }
}