#include "columnmux.hh"

#include <module/mux.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string ColumnMuxArguments::toString() const
    {
        return util::format("ss%d_ms%d", this->selectionSize, this->muxSize);
    }

    ColumnMux::ColumnMux(std::string name, ColumnMuxArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _selectionSize{arguments->selectionSize},
        _inputSize{util::power(2, arguments->selectionSize)},
        _muxSize{arguments->muxSize},
        _mux{nullptr}
    {
        this->createNetlist();
    }

    void ColumnMux::createPorts()
    {
        for (int i = 0; i < this->_selectionSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);
        
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
        {
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                this->addPort( util::format("bl%d_%d", muxIdx, inputIdx), PortType::INOUT);
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                this->addPort( util::format("br%d_%d", muxIdx, inputIdx), PortType::INOUT);
        }
        
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
            this->addPort(util::format("bl%d", muxIdx), PortType::INOUT);
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
            this->addPort(util::format("br%d", muxIdx), PortType::INOUT);

        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void ColumnMux::createCircuits()
    {
        MuxArguments muxArguments{this->_selectionSize};
        this->_mux = factory->create(ModuleType::MUX, &muxArguments);
        this->_circuits.emplace(this->_mux);
    } 

    void ColumnMux::createInstances() 
    {
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
        {
            Instance* mux{ this->addInstance(util::format("mux%d", muxIdx), this->_mux) };

            // S0 S1 ... Sn-1 bl0 bl1 ... bl2^n-1 br0 br1 ... br2^n-1 bl br vdd gnd
            std::vector<std::string> muxPorts{};
            for (int i = 0; i < this->_selectionSize; ++i)
                muxPorts.emplace_back(util::format("A%d", i));
            
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                muxPorts.emplace_back(util::format("bl%d_%d", muxIdx, inputIdx));
            
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                muxPorts.emplace_back(util::format("br%d_%d", muxIdx, inputIdx));

            muxPorts.emplace_back(util::format("bl%d", muxIdx));
            muxPorts.emplace_back(util::format("br%d", muxIdx));

            muxPorts.emplace_back("vdd");
            muxPorts.emplace_back("gnd");

            this->connectWith(mux, std::move(muxPorts));
        }
    }
}