#include "rowdecoder.hh"

#include <module/and.hh>
#include <module/decoder.hh>
#include <factory/circuitfactory.hh>

#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string RowDecoderArguments::toString() const
    {
        return util::format("as%d", this->addressSize);
    }

    RowDecoder::RowDecoder(std::string name, RowDecoderArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _addressSize{arguments->addressSize},
        _wlSize{util::power(2, this->_addressSize)},
        _and{nullptr},
        _decoder{nullptr}
    {
        this->createNetlist();
    }

    void RowDecoder::createPorts()
    {
        this->addPort("wl_en", PortType::INPUT);

        // Input Ports
        for (int i = 0; i < this->_addressSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);
        
        // Output Ports
        int outputSize{ util::power(2, this->_addressSize) };
        for (int i = 0; i < outputSize; ++i)
            this->addPort(util::format("wl%d", i), PortType::OUTPUT);

        // vdd & gnd
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void RowDecoder::createCircuits()
    {
        ANDArguments andArgument{DriveCapability::NORMAL, 2};
        this->_and = factory->create(ModuleType::AND, &andArgument);
        this->_circuits.emplace(this->_and);

        DecoderArguments decoderArgument{this->_addressSize};
        this->_decoder = factory->create(ModuleType::DECODER, &decoderArgument);
        this->_circuits.emplace(this->_decoder);
    } 

    void RowDecoder::createInstances() 
    {
        // Create decoder instance
        Instance* decoder{ this->addInstance("decoder", this->_decoder) };

        // Connect decoder
        std::vector<std::string> decoderPorts{};

        for (int i = 0; i < this->_addressSize; ++i)
            decoderPorts.emplace_back(util::format("A%d", i));

        std::vector<std::string> outputPorts{};
        for (int i = 0; i < this->_wlSize; ++i)
            decoderPorts.emplace_back( util::format("Y%d", i) );
        
        decoderPorts.emplace_back("vdd");
        decoderPorts.emplace_back("gnd");

        this->connectWith(decoder, std::move(decoderPorts));

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