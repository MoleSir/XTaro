#include "mux.hh"

#include <module/decoder.hh>
#include <module/inv.hh>
#include <module/trigate.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/format.hh>
#include <util/math.hh>
#include <exception/msgexception.hh>
#include <debug/logger.hh>

namespace xtaro::circuit
{
    std::string MuxArguments::toString() const
    {
        return util::format("ss%d", this->selectionSize);
    }

    Mux::Mux(String name, MuxArguments* arguments) :
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

            logger->error(errorMsg);
            throw MessageException {"Create Mux", errorMsg};
        }

        logger->debug("Create a 'Mux' circuit: '%s'", this->_name.cstr());
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
        this->_inv = factory->create(CircuitType::INV, &invArguments);
        this->_circuits.emplace(this->_inv);

        // Create decoder if necessary
        if (this->_selectionSize > 1)
        {
            DecoderArguments decoderArgument{this->_selectionSize};
            this->_decoder = factory->create(CircuitType::DECODER, &decoderArgument);
            this->_circuits.emplace(this->_decoder);
        }

        // Create tristate-gate
        this->_trigate = factory->create(CircuitType::TRISTATE_GATE, nullptr);
        this->_circuits.emplace(this->_trigate);
    }

    void Mux::createInstances() 
    {
        bool useDecoder {this->_selectionSize > 1};
        std::vector<String> decoderOutput{};
        std::vector<String> decoderOutputBar{};
        for (int i = 0; i < this->_inputSize; ++i)
        {
            decoderOutput.emplace_back(util::format("Y%d", i));
            decoderOutputBar.emplace_back(util::format("Y%d_bar", i));
        }

        // Create & Connect tristate-gate instance
        if (!useDecoder)
        {
            for (int i = 0; i < 2; ++i)
            {
                // in out en en_bar vdd gnd
                Instance* blTrigate{ this->addInstance(util::format("bl_tri_gate%d", i), this->_trigate) };
                Instance* brTrigate{ this->addInstance(util::format("br_tri_gate%d", i), this->_trigate) };

                this->connectWith(blTrigate, {
                    util::format("bl%d", i), "bl", 
                    i == 0 ? "S0" : "S0_bar", 
                    i == 0 ? "S0_bar" : "S0", 
                    "vdd", "gnd"
                });

                this->connectWith(brTrigate, {
                    util::format("br%d", i), "br", 
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
                Instance* blTrigate{ this->addInstance(util::format("bl_tri_gate%d", i), this->_trigate) };
                Instance* brTrigate{ this->addInstance(util::format("br_tri_gate%d", i), this->_trigate) };

                this->connectWith(blTrigate, {
                    util::format("bl%d", i), "bl", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
                });

                this->connectWith(brTrigate, {
                    util::format("br%d", i), "br", decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
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

            std::vector<String> decoderPorts{};
            for (int i = 0; i < this->_selectionSize; ++i)
                decoderPorts.emplace_back( util::format("S%d", i) );
            for (int i = 0; i < this->_inputSize; ++i)
                decoderPorts.emplace_back( decoderOutput[i] );
            decoderPorts.emplace_back("vdd");
            decoderPorts.emplace_back("gnd");

            this->connectWith(decoder, decoderPorts);

            // INV gates
            for (int i = 0; i < this->_inputSize; ++i)
            {
                Instance* inv{ this->addInstance(util::format("inv%d", i), this->_inv) };
                this->connectWith(inv, {
                    decoderOutput[i], decoderOutputBar[i], "vdd", "gnd"
                });
            }
        }
    }
}