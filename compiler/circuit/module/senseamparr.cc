#include "senseamparr.hh"

#include <module/senseamp.hh>
#include <module/fanoutbuff.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <exception/msgexception.hh>
#include <util/util.hh>
#include <log/logger.hh>

namespace xtaro::circuit
{
    std::string SenseAmplifierArrayArguments::toString() const
    {
        return util::format("ww%d", this->wordWidth);
    }

    SenseAmplifierArray::SenseAmplifierArray(String name, SenseAmplifierArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _wordWidth{arguments->wordWidth},
        _senseamp{nullptr},
        _fanoutSize{0},
        _fanoutbuf{nullptr}
    {
        if (this->_wordWidth < 1)
        {
            std::string errorMsg {util::format("Sense Amplifier Array's word width '%d' < 1", this->_wordWidth)};

            logger->error(errorMsg);
            throw MessageException("Create Sense Amplifier Array", errorMsg);
        }

        this->_fanoutSize = this->_wordWidth / MAX_FANOUT;
        if (this->_wordWidth % MAX_FANOUT != 0) this->_fanoutSize += 1;
        
        logger->debug("Create a 'Sense Amplifier Array' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void SenseAmplifierArray::createPorts()
    {
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("bl%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("br%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("dout%d", i), PortType::OUTPUT);
        
        this->addPort("sa_en", PortType::INPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void SenseAmplifierArray::createCircuits()
    {
        this->_senseamp = factory->create(ModuleType::SENSE_AMPLIFIER, nullptr);
        this->_circuits.emplace(this->_senseamp);
        
        // If word width is too much, 'sa_en''s fan-out too much...
        if (this->_fanoutSize > 1)
        {
            FanoutBufferArguments fanoutArguments {this->_fanoutSize};
            this->_fanoutbuf = factory->create(ModuleType::FANOUT_BUFFER, &fanoutArguments);
            this->_circuits.emplace(this->_fanoutbuf);
        }
    } 

    void SenseAmplifierArray::createInstances() 
    {
        if (this->_fanoutSize > 1)
        {
            Instance* fanoutbuff {this->addInstance("fanout_buff", this->_fanoutbuf)};

            std::vector<String> ports {};
            ports.emplace_back("sa_en");
            for (int i = 0; i < this->_fanoutSize; ++i)
                ports.emplace_back(util::format("sa_en%d", i));
            ports.emplace_back("vdd");
            ports.emplace_back("gnd");

            this->connectWith(fanoutbuff, ports);
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            Instance* senseamp {
                this->addInstance(util::format("senseamp%d", i), this->_senseamp)
            };

            this->connectWith(senseamp, {
                util::format("bl%d", i), 
                util::format("br%d", i), 
                util::format("dout%d", i),
                this->_fanoutSize > 1 ? util::format("sa_en%d", i / MAX_FANOUT) : "sa_en",
                "vdd", "gnd"
            });
        }
    }
}