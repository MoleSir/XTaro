#include "senseamparr.hh"

#include <module/senseamp.hh>
#include <module/fanoutbuff.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string SenseAmplifierArrayArguments::toString() const
    {
        return util::format("ww%d", this->wordWidth);
    }

    SenseAmplifierArray::SenseAmplifierArray(const std::string_view& name, SenseAmplifierArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _wordWidth{arguments->wordWidth},
        _senseamp{nullptr},
        _fanoutSize{0},
        _fanoutbuf{nullptr}
    {
        if (this->_wordWidth < 1)
        {
            std::string errorMsg {util::format("Sense Amplifier Array's word width '%d' < 1", this->_wordWidth)};
            debug->errorWithException("Create Sense Amplifier Array", errorMsg);
        }

        this->_fanoutSize = this->_wordWidth / MAX_FANOUT;
        if (this->_wordWidth % MAX_FANOUT != 0) this->_fanoutSize += 1;
        
        debug->debug("Create a 'Sense Amplifier Array' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void SenseAmplifierArray::createPorts()
    {
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("bl%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("br%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("dout%d", i), PortType::OUTPUT);
        
        this->addPort("sa_en", PortType::INPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void SenseAmplifierArray::createCircuits()
    {
        this->_senseamp = this->addCircuit("sense_amp", nullptr);
        
        // If word width is too much, 'sa_en''s fan-out too much...
        if (this->_fanoutSize > 1)
        {
            FanoutBufferArguments fanoutArguments {this->_fanoutSize};
            this->_fanoutbuf = this->addCircuit("fanout_buffer", &fanoutArguments);
        }
    } 

    void SenseAmplifierArray::createInstances() 
    {
        if (this->_fanoutSize > 1)
        {
            Instance* fanoutbuff {this->addInstance("fanout_buff", this->_fanoutbuf)};

            std::vector<std::string_view> ports {};
            ports.emplace_back("sa_en");
            for (int i = 0; i < this->_fanoutSize; ++i)
                ports.emplace_back(stringFactory->get("sa_en%d", i));
            ports.emplace_back("vdd");
            ports.emplace_back("gnd");

            this->connectWith(fanoutbuff, ports);
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            Instance* senseamp {
                this->addInstance(stringFactory->get("senseamp%d", i), this->_senseamp)
            };

            this->connectWith(senseamp, {
                stringFactory->get("bl%d", i), 
                stringFactory->get("br%d", i), 
                stringFactory->get("dout%d", i),
                this->_fanoutSize > 1 ? stringFactory->get("sa_en%d", i / MAX_FANOUT) : "sa_en",
                "vdd", "gnd"
            });
        }
    }
}