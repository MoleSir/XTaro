#include "mux.hh"

#include <module/decoder.hh>
#include <module/inv.hh>
#include <module/trigate.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>
#include <exception/msgexception.hh>

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
        // TODO: Check selection size
        if (this->_selectionSize < 1 || this->_selectionSize > Mux::MAX_SELECTION_SIZE)
            throw MessageException(
                "Create Mux", 
                 util::format("'%d' is not a vaild selection size", this->_selectionSize)
            );

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
        // Create INV
        INVArguments invArguments{DriveCapability::NORMAL};
        this->_inv = factory->create(ModuleType::INV, &invArguments);
        this->_circuits.emplace(this->_inv);

        // Create decoder if necessary
        if (this->_selectionSize > 1)
        {
            DecoderArguments decoderArgument{this->_selectionSize};
            this->_decoder = factory->create(ModuleType::DECODER, &decoderArgument);
            this->_circuits.emplace(this->_decoder);
        }

        // Create tristate-gate
        this->_trigate = factory->create(ModuleType::TRISTATE_GATE, nullptr);
        this->_circuits.emplace(this->_trigate);
    }

    void Mux::createInstances() 
    {
        std::vector<std::string> decoderOutput{};
        std::vector<std::string> decoderOutputBar{};
        for (int i = 0; i < this->_inputSize; ++i)
        {
            decoderOutput.emplace_back(util::format("Y%d", i));
            decoderOutputBar.emplace_back(util::format("Y%d_bar", i));
        }

        // Create & Connect tristate-gate instance
        for (int i = 0; i < this->_inputSize; ++i)
        {
            // in out en en_bar vdd gnd
            Instance* blTrigate{ this->addInstance(util::format("bl_tri_gate%d", i), this->_trigate) };
            Instance* brTrigate{ this->addInstance(util::format("br_tri_gate%d", i), this->_trigate) };

            this->connectWith(blTrigate, {
                util::format("bl%d", i), "bl", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
            });

            this->connectWith(brTrigate, {
                util::format("br%d", i), "br", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
            });
        }

        // Create decoder instance if necessary
        if (this->_selectionSize == 1)
        {
            // No need decoder, just a INV
            Instance* inv{ this->addInstance("inv", this->_inv) };
            this->connectWith(inv, {"S0", "Y0", "Y0_bar", "vdd", "gnd"});
        }
        else
        {
            // Need decoder
            // A0 A1 ... An Y0 Y1 ... Y2^n-1 vdd gnd
            Instance* decoder{ this->addInstance("decoder", this->_decoder) };

            std::vector<std::string> decoderPorts{};
            for (int i = 0; i < this->_selectionSize; ++i)
                decoderPorts.emplace_back( util::format("S%d", i) );
            for (int i = 0; i < this->_inputSize; ++i)
                decoderPorts.emplace_back( decoderOutput[i] );
            decoderPorts.emplace_back("vdd");
            decoderPorts.emplace_back("gnd");

            this->connectWith(decoder, std::move(decoderPorts));

            // INV gates
            for (int i = 0; i < this->_inputSize; ++i)
            {
                Instance* inv{ this->addInstance(util::format("inv%d", i), this->_inv) };
                this->connectWith(inv, {
                    std::move(decoderOutput[i]), std::move(decoderOutputBar[i]), "vdd", "gnd"
                });
            }
        }
    }
}