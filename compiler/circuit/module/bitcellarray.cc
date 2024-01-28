#include "bitcellarray.hh"
#include <base/circuit.hh>

#include <factory/stringfactory.hh>

#include <config/tech.hh>
#include <util/format.hh>
#include <debug/debug.hh>

#include <cmath>

namespace xtaro::circuit
{
    std::string BitcellArrayArguments::toString() const
    {
        return util::format("r%d_c%d", this->rowSize, this->columnSize);
    }

    BitcellArray::BitcellArray(const std::string_view& name, BitcellArrayArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _rowSize{arguments->rowSize},
        _columnSize{arguments->columnSize},
        _bitcell{nullptr}
    {
        if (this->_rowSize < 1 || this->_columnSize < 1)
        {
            std::string errorMsg {
                util::format(
                    "Bitcell Array's row size '%d' or column size '%d'< 1", 
                    this->_rowSize, this->_columnSize
                )
            };

            debug->errorWithException("Create Bitcell Array", errorMsg);
        }

        debug->debug("Create a 'Bitcell Array' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void BitcellArray::createPorts()
    {
        // 'bl' and 'br'
        std::vector<std::string_view> blbrPortsName;
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( stringFactory->get("bl%d", i) );
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( stringFactory->get("br%d", i) );

        this->addPorts(blbrPortsName, PortType::OUTPUT);

        // 'WL' ports
        std::vector<std::string_view> wlPortsName;
        for (std::int32_t i = 0; i < this->_rowSize; ++i)
            wlPortsName.emplace_back( stringFactory->get("wl%d", i) );
        
        this->addPorts(wlPortsName, PortType::INPUT);

        // 'vdd' and 'gnd'
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void BitcellArray::createCircuits()
    {
        // Create 'bitcell' circuit
        this->_bitcell = this->addCircuit("bitcell", nullptr);
    }

    void BitcellArray::createInstances()
    {
        for (std::int32_t r = 0; r < this->_rowSize; ++r)
        {
            for (std::int32_t c = 0; c < this->_columnSize; ++c)
            {
                Instance* instance{ this->addInstance(
                    stringFactory->get("bitcell_%d_%d", r, c),
                    this->_bitcell
                ) };
                
                this->connectWith(instance, {
                    stringFactory->get("bl%d", c),
                    stringFactory->get("br%d", c),
                    stringFactory->get("wl%d", r),
                    "vdd",
                    "gnd"
                });
            }
        }
    }
}