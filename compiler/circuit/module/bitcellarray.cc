#include "bitcellarray.hh"
#include <base/circuit.hh>
#include <allocator/allocator.hh>
#include <factory/circuitfactory.hh>

#include <config/tech.hh>
#include <util/format.hh>
#include <debug/logger.hh>
#include <debug/debug.hh>

#include <cmath>

namespace xtaro::circuit
{
    std::string BitcellArrayArguments::toString() const
    {
        return util::format("r%d_c%d", this->rowSize, this->columnSize);
    }

    BitcellArray::BitcellArray(String name, BitcellArrayArguments* arguments) :
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

            debug->reportError("Create Bitcell Array", errorMsg);
        }

        logger->debug("Create a 'Bitcell Array' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void BitcellArray::createPorts()
    {
        // 'bl' and 'br'
        std::vector<String> blbrPortsName;
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( util::format("bl%d", i) );
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( util::format("br%d", i) );

        this->addPorts(blbrPortsName, PortType::OUTPUT);

        // 'WL' ports
        std::vector<String> wlPortsName;
        for (std::int32_t i = 0; i < this->_rowSize; ++i)
            wlPortsName.emplace_back( util::format("wl%d", i) );
        
        this->addPorts(wlPortsName, PortType::INPUT);

        // 'vdd' and 'gnd'
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void BitcellArray::createCircuits()
    {
        // Create 'bitcell' circuit
        this->_bitcell = factory->create(CircuitType::BITCELL, nullptr);
        this->_circuits.insert(this->_bitcell);
    }

    void BitcellArray::createInstances()
    {
        for (std::int32_t r = 0; r < this->_rowSize; ++r)
        {
            for (std::int32_t c = 0; c < this->_columnSize; ++c)
            {
                Instance* instance{ this->addInstance(
                    util::format("bitcell_%d_%d", r, c),
                    this->_bitcell
                ) };
                
                this->connectWith(instance, {
                    util::format("bl%d", c),
                    util::format("br%d", c),
                    util::format("wl%d", r),
                    "vdd",
                    "gnd"
                });
            }
        }
    }
}