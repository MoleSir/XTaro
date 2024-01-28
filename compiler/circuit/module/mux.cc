#include "mux.hh"

#include <module/decoder.hh>
#include <module/inv.hh>
#include <module/trigate.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <util/math.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string MuxArguments::toString() const
    {
        return util::format("ss%d", this->selectionSize);
    }

    Mux::Mux(const std::string_view& name, MuxArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _selectionSize{arguments->selectionSize},
        _inputSize{util::power(2, arguments->selectionSize)},
        _decoder{nullptr},
        _trigate{nullptr},
        _inv{nullptr}
    {
        if (this->_selectionSize < 1 || this->_selectionSize > Mux::MAX_SELECTION_SIZE)
        {
            std::string errorMsg {util::format("Mux's selection size '%d' is not a vaild", this->_selectionSize)};
            debug->errorWithException("Create Mux", errorMsg);
        }

        debug->debug("Create a 'Mux' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void Mux::createPorts()
    {
        for (int i = 0; i < this->_selectionSize; ++i)
            this->addPort(stringFactory->get("S%d", i), PortType::INPUT);

        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(stringFactory->get("bl%d", i), PortType::INOUT);
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(stringFactory->get("br%d", i), PortType::INOUT);

        this->addPort("bl", PortType::INOUT);
        this->addPort("br", PortType::INOUT);
    
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void Mux::createCircuits()
    {
        // Create INV
        INVArguments invArguments{DriveCapability::NORMAL};
        this->_inv = this->addCircuit("inv", &invArguments);

        // Create decoder if necessary
        if (this->_selectionSize > 1)
        {
            DecoderArguments decoderArgument{this->_selectionSize};
            this->_decoder = this->addCircuit("decoder", &decoderArgument);
        }

        // Create tristate-gate
        this->_trigate = this->addCircuit("tristate_gate", nullptr);
    }

    void Mux::createInstances() 
    {
        bool useDecoder {this->_selectionSize > 1};
        std::vector<std::string_view> decoderOutput{};
        std::vector<std::string_view> decoderOutputBar{};
        for (int i = 0; i < this->_inputSize; ++i)
        {
            decoderOutput.emplace_back(stringFactory->get("Y%d", i));
            decoderOutputBar.emplace_back(stringFactory->get("Y%d_bar", i));
        }

        // Create & Connect tristate-gate instance
        if (!useDecoder)
        {
            for (int i = 0; i < 2; ++i)
            {
                // in out en en_bar vdd gnd
                Instance* blTrigate{ this->addInstance(stringFactory->get("bl_tri_gate%d", i), this->_trigate) };
                Instance* brTrigate{ this->addInstance(stringFactory->get("br_tri_gate%d", i), this->_trigate) };

                this->connectWith(blTrigate, {
                    stringFactory->get("bl%d", i), "bl", 
                    i == 0 ? "S0" : "S0_bar", 
                    i == 0 ? "S0_bar" : "S0", 
                    "vdd", "gnd"
                });

                this->connectWith(brTrigate, {
                    stringFactory->get("br%d", i), "br", 
                    i == 0 ? "S0" : "S0_bar", 
                    i == 0 ? "S0_bar" : "S0", 
                    "vdd", "gnd"
                });
            }
        }
        else
        {
            for (int i = 0; i < this->_inputSize; ++i)
            {
                // in out en en_bar vdd gnd
                Instance* blTrigate{ this->addInstance(stringFactory->get("bl_tri_gate%d", i), this->_trigate) };
                Instance* brTrigate{ this->addInstance(stringFactory->get("br_tri_gate%d", i), this->_trigate) };

                this->connectWith(blTrigate, {
                    stringFactory->get("bl%d", i), "bl", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
                });

                this->connectWith(brTrigate, {
                    stringFactory->get("br%d", i), "br", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
                });
            }
        }

        // Create decoder instance if necessary
        if (!useDecoder)
        {
            // No need decoder, just a INV
            Instance* inv {this->addInstance("inv", this->_inv)};
            this->connectWith(inv, {"S0", "S0_bar", "vdd", "gnd"});
        }
        else
        {
            // Need decoder
            // A0 A1 ... An Y0 Y1 ... Y2^n-1 vdd gnd
            Instance* decoder{ this->addInstance("decoder", this->_decoder) };

            std::vector<std::string_view> decoderPorts{};
            for (int i = 0; i < this->_selectionSize; ++i)
                decoderPorts.emplace_back( stringFactory->get("S%d", i) );
            for (int i = 0; i < this->_inputSize; ++i)
                decoderPorts.emplace_back( decoderOutput[i] );
            decoderPorts.emplace_back("vdd");
            decoderPorts.emplace_back("gnd");

            this->connectWith(decoder, decoderPorts);

            // INV gates
            for (int i = 0; i < this->_inputSize; ++i)
            {
                Instance* inv{ this->addInstance(stringFactory->get("inv%d", i), this->_inv) };
                this->connectWith(inv, {
                    decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
                });
            }
        }
    }
}