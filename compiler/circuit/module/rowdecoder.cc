#include "rowdecoder.hh"

#include <module/and.hh>
#include <module/decoder.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <util/math.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string RowDecoderArguments::toString() const
    {
        return util::format("as%d", this->addressSize);
    }

    RowDecoder::RowDecoder(const std::string_view& name, RowDecoderArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressSize{arguments->addressSize},
        _wlSize{util::power(2, this->_addressSize)},
        _and{nullptr},
        _decoder{nullptr}
    {
        if (this->_addressSize < 2)
        {
            std::string errorMsg {util::format("'Row Decoder's address size '%d' < 2", this->_addressSize)};
            debug->errorWithException("Create 'Row Decoder", errorMsg);
        }

        debug->debug("Create a 'Row Decoder' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void RowDecoder::createPorts()
    {
        this->addPort("wl_en", PortType::INPUT);

        // Input Ports
        for (int i = 0; i < this->_addressSize; ++i)
            this->addPort(stringFactory->get("A%d", i), PortType::INPUT);
        
        // Output Ports
        for (int i = 0; i < this->_wlSize; ++i)
            this->addPort(stringFactory->get("wl%d", i), PortType::OUTPUT);

        // vdd & gnd
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void RowDecoder::createCircuits()
    {
        ANDArguments andArgument{DriveCapability::NORMAL, 2};
        this->_and = this->addCircuit("and", &andArgument);

        DecoderArguments decoderArgument{this->_addressSize};
        this->_decoder = this->addCircuit("decoder", &decoderArgument);
    } 

    void RowDecoder::createInstances() 
    {
        // Create decoder instance
        Instance* decoder{ this->addInstance("decoder", this->_decoder) };

        // Connect decoder
        std::vector<std::string_view> decoderPorts{};

        for (int i = 0; i < this->_addressSize; ++i)
            decoderPorts.emplace_back(stringFactory->get("A%d", i));

        std::vector<std::string_view> outputPorts{};
        for (int i = 0; i < this->_wlSize; ++i)
            decoderPorts.emplace_back( stringFactory->get("Y%d", i) );
        
        decoderPorts.emplace_back("vdd");
        decoderPorts.emplace_back("gnd");

        this->connectWith(decoder, decoderPorts);

        // Create AND gates 
        for (int i = 0; i < this->_wlSize; ++i)
        {
            Instance* andGate{ this->addInstance(stringFactory->get("and%d", i), this->_and) };
            this->connectWith(andGate, {
                "wl_en", stringFactory->get("Y%d", i), stringFactory->get("wl%d", i), "vdd", "gnd"
            });
        }
    }
}