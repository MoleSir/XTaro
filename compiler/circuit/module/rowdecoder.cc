#include "rowdecoder.hh"

#include <module/and.hh>
#include <module/decoder.hh>
#include <factory/circuitfactory.hh>

#include <allocator/allocator.hh>
#include <util/format.hh>
#include <util/math.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string RowDecoderArguments::toString() const
    {
        return util::format("as%d", this->addressSize);
    }

    RowDecoder::RowDecoder(String name, RowDecoderArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressSize{arguments->addressSize},
        _wlSize{util::power(2, this->_addressSize)},
        _and{nullptr},
        _decoder{nullptr}
    {
        if (this->_addressSize < 2)
        {
            std::string errorMsg {util::format("'Row Decoder's address size '%d' < 2", this->_addressSize)};
            debug->error("Create 'Row Decoder", errorMsg);
        }

        debug->debug("Create a 'Row Decoder' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void RowDecoder::createPorts()
    {
        this->addPort("wl_en", PortType::INPUT);

        // Input Ports
        for (int i = 0; i < this->_addressSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);
        
        // Output Ports
        for (int i = 0; i < this->_wlSize; ++i)
            this->addPort(util::format("wl%d", i), PortType::OUTPUT);

        // vdd & gnd
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void RowDecoder::createCircuits()
    {
        ANDArguments andArgument{DriveCapability::NORMAL, 2};
        this->_and = factory->create(CircuitType::AND, &andArgument);
        this->_circuits.emplace(this->_and);

        DecoderArguments decoderArgument{this->_addressSize};
        this->_decoder = factory->create(CircuitType::DECODER, &decoderArgument);
        this->_circuits.emplace(this->_decoder);
    } 

    void RowDecoder::createInstances() 
    {
        // Create decoder instance
        Instance* decoder{ this->addInstance("decoder", this->_decoder) };

        // Connect decoder
        std::vector<String> decoderPorts{};

        for (int i = 0; i < this->_addressSize; ++i)
            decoderPorts.emplace_back(util::format("A%d", i));

        std::vector<String> outputPorts{};
        for (int i = 0; i < this->_wlSize; ++i)
            decoderPorts.emplace_back( util::format("Y%d", i) );
        
        decoderPorts.emplace_back("vdd");
        decoderPorts.emplace_back("gnd");

        this->connectWith(decoder, decoderPorts);

        // Create AND gates 
        for (int i = 0; i < this->_wlSize; ++i)
        {
            Instance* andGate{ this->addInstance(util::format("and%d", i), this->_and) };
            this->connectWith(andGate, {
                "wl_en", util::format("Y%d", i), util::format("wl%d", i), "vdd", "gnd"
            });
        }
    }
}