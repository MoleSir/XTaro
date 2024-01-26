#include "writedriverarr.hh"

#include <module/writedriver.hh>
#include <module/fanoutbuff.hh>

#include <factory/factory.hh>
#include <allocator/allocator.hh>
#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string WriteDriverArrayArguments::toString() const
    {
        return util::format("ww%d", this->wordWidth);
    }

    WriteDriverArray::WriteDriverArray(String name, WriteDriverArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _wordWidth{arguments->wordWidth},
        _fanoutSize{0},
        _writedriver{nullptr},
        _fanoutbuf{nullptr}
    {
        if (this->_wordWidth < 1)
        {
            std::string errorMsg {util::format("Write Driver Array's word width '%d' < 1", this->_wordWidth)};
            debug->errorWithException("Create Write Driver Array", errorMsg);
        }

        this->_fanoutSize = this->_wordWidth / MAX_FANOUT;
        if (this->_wordWidth % MAX_FANOUT != 0) this->_fanoutSize += 1;
        
        debug->debug("Create a 'Write Driver Array' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void WriteDriverArray::createPorts()
    {
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("din%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("bl%d", i), PortType::INOUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("br%d", i), PortType::INOUT);
        
        this->addPort("we_en", PortType::INPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void WriteDriverArray::createCircuits()
    {
        this->_writedriver = factory->create("write_driver", nullptr);
        this->_circuits.emplace(this->_writedriver);
        
        // If word width is too much, 'we_en''s fan-out too much...
        if (this->_fanoutSize > 1)
        {
            FanoutBufferArguments fanoutArguments {this->_fanoutSize};
            this->_fanoutbuf = factory->create("fanout_buffer", &fanoutArguments);
            this->_circuits.emplace(this->_fanoutbuf);
        }
    } 

    void WriteDriverArray::createInstances() 
    {
        if (this->_fanoutSize > 1)
        {
            Instance* fanoutbuff {this->addInstance("fanout_buff", this->_fanoutbuf)};

            std::vector<String> ports {};
            ports.emplace_back("we_en");
            for (int i = 0; i < this->_fanoutSize; ++i)
                ports.emplace_back(util::format("we_en%d", i));
            ports.emplace_back("vdd");
            ports.emplace_back("gnd");

            this->connectWith(fanoutbuff, ports);
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            Instance* writedriver {
                this->addInstance(util::format("write_driver%d", i), this->_writedriver)
            };

            this->connectWith(writedriver, {
                util::format("din%d", i),
                util::format("bl%d", i), 
                util::format("br%d", i), 
                this->_fanoutSize > 1 ? util::format("we_en%d", i / MAX_FANOUT) : "we_en",
                "vdd", "gnd"
            });
        }
    }
}