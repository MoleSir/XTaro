#include "prechargearr.hh"

#include <module/precharge.hh>
#include <module/fanoutbuff.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string PrechargeArrayArguments::toString() const
    {
        return util::format("ww%d", this->wordWidth);
    }

    PrechargeArray::PrechargeArray(const std::string_view& name, PrechargeArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _wordWidth{arguments->wordWidth},
        _precharge{nullptr},
        _fanoutSize{0},
        _fanoutbuf{nullptr}
    {
        if (this->_wordWidth < 1)
        {
            std::string errorMsg {util::format("Precharge Array's word width '%d' < 1", this->_wordWidth)};
            debug->errorWithException("Create Precharge Array", errorMsg);
        }

        this->_fanoutSize = this->_wordWidth / MAX_FANOUT;
        if (this->_wordWidth % MAX_FANOUT != 0) this->_fanoutSize += 1;
        
        debug->debug("Create a 'Precharge Array' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void PrechargeArray::createPorts()
    {
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("bl%d", i), PortType::INOUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("br%d", i), PortType::INOUT);
        
        this->addPort("p_en_bar", PortType::INPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void PrechargeArray::createCircuits()
    {
        this->_precharge = this->addCircuit("precharge", nullptr);
        
        // If word width is too much, 'p_en_bar''s fan-out too much...
        if (this->_fanoutSize > 1)
        {
            FanoutBufferArguments fanoutArguments {this->_fanoutSize};
            this->_fanoutbuf = this->addCircuit("fanout_buffer", &fanoutArguments);
        }
    } 

    void PrechargeArray::createInstances() 
    {
        if (this->_fanoutSize > 1)
        {
            Instance* fanoutbuff {this->addInstance("fanout_buff", this->_fanoutbuf)};

            std::vector<std::string_view> ports {};
            ports.emplace_back("p_en_bar");
            for (int i = 0; i < this->_fanoutSize; ++i)
                ports.emplace_back(stringFactory->get("p_en_bar%d", i));
            ports.emplace_back("vdd");
            ports.emplace_back("gnd");

            this->connectWith(fanoutbuff, ports);
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            Instance* writedriver {
                this->addInstance(stringFactory->get("precharge%d", i), this->_precharge)
            };

            this->connectWith(writedriver, {
                stringFactory->get("bl%d", i), 
                stringFactory->get("br%d", i), 
                this->_fanoutSize > 1 ? stringFactory->get("p_en_bar%d", i / MAX_FANOUT) : "p_en_bar",
                "vdd", "gnd"
            });
        }
    }
}