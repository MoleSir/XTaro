#include "columnmux.hh"

#include <module/mux.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <util/math.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string ColumnMuxArguments::toString() const
    {
        return util::format("ss%d_ms%d", this->selectionSize, this->muxSize);
    }

    ColumnMux::ColumnMux(const std::string_view& name, ColumnMuxArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _selectionSize{arguments->selectionSize},
        _inputSize{util::power(2, arguments->selectionSize)},
        _muxSize{arguments->muxSize},
        _mux{nullptr}
    {
        if (this->_selectionSize < 1)
        {
            std::string errorMsg {util::format("Column Mux's selection size '%d' < 1", this->_selectionSize)};
            debug->errorWithException("Create Column Mux", errorMsg);
        }

        debug->debug("Create a 'Column Mux' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void ColumnMux::createPorts()
    {
        for (int i = 0; i < this->_selectionSize; ++i)
            this->addPort(stringFactory->get("A%d", i), PortType::INPUT);
        
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
        {
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                this->addPort( stringFactory->get("bl%d_%d", muxIdx, inputIdx), PortType::INOUT);
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                this->addPort( stringFactory->get("br%d_%d", muxIdx, inputIdx), PortType::INOUT);
        }
        
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
            this->addPort(stringFactory->get("bl%d", muxIdx), PortType::INOUT);
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
            this->addPort(stringFactory->get("br%d", muxIdx), PortType::INOUT);

        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void ColumnMux::createCircuits()
    {
        MuxArguments muxArguments{this->_selectionSize};
        this->_mux = this->addCircuit("mux", &muxArguments);
    } 

    void ColumnMux::createInstances() 
    {
        for (int muxIdx = 0; muxIdx < this->_muxSize; ++muxIdx)
        {
            Instance* mux { 
                this->addInstance(stringFactory->get("mux%d", muxIdx), this->_mux) 
            };
 
            // S0 S1 ... Sn-1 bl0 bl1 ... bl2^n-1 br0 br1 ... br2^n-1 bl br vdd gnd
            std::vector<std::string_view> muxPorts{};
            for (int i = 0; i < this->_selectionSize; ++i)
                muxPorts.emplace_back(stringFactory->get("A%d", i));
            
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                muxPorts.emplace_back(stringFactory->get("bl%d_%d", muxIdx, inputIdx));
            
            for (int inputIdx = 0; inputIdx < this->_inputSize; ++inputIdx)
                muxPorts.emplace_back(stringFactory->get("br%d_%d", muxIdx, inputIdx));

            muxPorts.emplace_back(stringFactory->get("bl%d", muxIdx));
            muxPorts.emplace_back(stringFactory->get("br%d", muxIdx));

            muxPorts.emplace_back("vdd");
            muxPorts.emplace_back("gnd");

            this->connectWith(mux, muxPorts);
        }
    }
}