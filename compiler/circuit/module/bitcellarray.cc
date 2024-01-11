#include "bitcellarray.hh"
#include <base/circuit.hh>
#include <allocator/allocator.hh>
#include <factory/circuitfactory.hh>

#include <tech/tech.hh>
#include <util/util.hh>

#include <cmath>

namespace xtaro::circuit
{

    BitcellArray::BitcellArray(std::string name, BitcellArrayArguments* arguments) :
        Circuit{std::move(name)},
        _wordWidth{ arguments->wordWidth },
        _addressWidth{ arguments->addressWidth },
        _rowSize{0},
        _columnSize{0},
        _bitcell{nullptr}
    {
        // TODO: Set suitable row and column size
        this->_rowSize = std::pow(2, this->_addressWidth);
        this->_columnSize = arguments->wordWidth;

        this->createNetlist();
    }

    void BitcellArray::createPorts()
    {
        // 'bl' and 'br'
        std::vector<std::string> blbrPortsName;
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( util::format("bl%d", i) );
        for (std::int32_t i = 0; i < this->_columnSize; ++i)
            blbrPortsName.emplace_back( util::format("br%d", i) );

        this->addPorts( std::move(blbrPortsName), PortType::OUTPUT );

        // 'WL' ports
        std::vector<std::string> wlPortsName;
        for (std::int32_t i = 0; i < this->_rowSize; ++i)
            wlPortsName.emplace_back( util::format("wl%d", i) );
        
        this->addPorts( std::move(wlPortsName), PortType::INPUT );

        // 'vdd' and 'gnd'
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void BitcellArray::createCircuits()
    {
        // Create 'bitcell' circuit
        this->_bitcell = factory->create(CircuitType::BITCELL, "bitcell");
        this->_circuits.insert(this->_bitcell);
    }

    void BitcellArray::createInstances()
    {
        for (std::int32_t r = 0; r < this->_rowSize; ++r)
        {
            for (std::int32_t c = 0; c < this->_columnSize; ++c)
            {
                Instance* instance{ Allocator::alloc<Instance>(
                    util::format("bitcell_%d_%d", r, c),
                    this->_bitcell
                ) };
                this->_instances.emplace_back(instance);
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