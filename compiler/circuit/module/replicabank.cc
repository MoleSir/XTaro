#include "replicabank.hh"

#include <module/bitcell.hh>
#include <module/precharge.hh>
#include <module/writedriver.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>
#include <exception/msgexception.hh>
#include <log/logger.hh>

namespace xtaro::circuit
{
    std::string ReplicaBankArguments::toString() const
    {
        return util::format("bs%d", this->bitcellSize);
    }

    ReplicaBank::ReplicaBank(String name, ReplicaBankArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _bitcellSize{arguments->bitcellSize},
        _bitcell{nullptr},
        _prechage{nullptr},
        _writedriver{nullptr}
    {
        if (this->_bitcellSize < LINKED_BITCELL_SIZE)
        {
            std::string errorMsg {
                util::format(
                    "Replica Bank's bitcell size '%d' < %d", 
                    this->_bitcellSize, LINKED_BITCELL_SIZE
                )
            };

            logger->error(errorMsg);
            throw MessageException {"Create Replica Bank", errorMsg};
        }

        logger->debug("Create a 'Replica Bank' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void ReplicaBank::createPorts()
    {
        this->addPort("wl_en", PortType::INPUT);
        this->addPort("p_en_bar", PortType::INPUT);
        this->addPort("sa_en", PortType::INPUT);
        this->addPort("we_en", PortType::INPUT);

        this->addPort("rbl", PortType::INOUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void ReplicaBank::createCircuits()
    {   
        this->_bitcell = factory->create(ModuleType::BITCELL, nullptr);
        this->_circuits.emplace(this->_bitcell);

        this->_prechage = factory->create(ModuleType::PRECHARGE, nullptr);
        this->_circuits.emplace(this->_prechage);

        this->_writedriver = factory->create(ModuleType::WRITE_DRIVER, nullptr);
        this->_circuits.emplace(this->_writedriver);
    } 

    void ReplicaBank::createInstances() 
    {
        // Create bitcells
        for (int i = 0; i < this->_bitcellSize; ++i)
        {
            // bl br wl vdd gnd
            Instance* bitcell {this->addInstance(util::format("bicell%d", i), this->_bitcell)};
            this->connectWith(bitcell, {
                "rbl", "rbr",
                i < LINKED_BITCELL_SIZE ? "wl_en" : "gnd",
                "vdd", "gnd"
            });
        }

        // Create outside circuits
        // bl br en_bar vdd gnd
        Instance* precharge {this->addInstance("precharge", this->_prechage)};
        this->connectWith(precharge, {"rbl", "rbr", "p_en_bar", "vdd", "gnd"});

        // din bl br en vdd gnd
        Instance* writedriver {this->addInstance("writedriver", this->_writedriver)};
        this->connectWith(writedriver, {"gnd", "rbl", "rbr", "we_en", "vdd", "gnd"});  
    }
}