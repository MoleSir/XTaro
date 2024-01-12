#include "circuitfactory.hh"
#include <allocator/allocator.hh>
#include <tech/tech.hh>

#include <base/circuit.hh>
#include <module/bitcell.hh>
#include <module/bitcellarray.hh>
#include <module/mos.hh>

#include <util/util.hh>

namespace xtaro::circuit
{
    #define INT(v) static_cast<int>(v)

    const std::array<const char*, MODULESIZE> CircuitFactory::modulesName 
    {   
        "bitcell", 
        "bitcell_array",
        "", // MOS name is special!!!
    };

    CircuitFactory* factory{CircuitFactory::instance()};

    Circuit* CircuitFactory::create(ModuleType circuitType, 
                                    CircuitArguments* arguments, 
                                    std::string circuitName)
    {
        // Is target circuit exits
        std::string argsList{ arguments == nullptr ? "" : arguments->toString() };
        Circuit* circuit{this->findCircuit(circuitType, argsList)};
        if (circuit != nullptr)
            return circuit;

        // Get circuit's name
        if (circuitName.empty())
            circuitName = this->getDefaultCircuitName(circuitType);

        // Create a new circuit'
        circuit = this->createNewCircuit(circuitType, arguments, std::move(circuitName));

        // Collect circuit
        this->collectCircuit(circuitType, argsList, circuit);

        return circuit;
    }

    CircuitFactory* CircuitFactory::instance()
    {
        static CircuitFactory _factory;
        return &_factory;
    }

    Circuit* CircuitFactory::findCircuit(ModuleType circuitType, 
                                         const std::string& argsList) const
    {   
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        auto iter{ circuits.find(argsList) };
        if (iter != circuits.end())
            return iter->second;
        return nullptr;
    }

    std::string CircuitFactory::getDefaultCircuitName(ModuleType circuitType) const
    {
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        if (circuitType == ModuleType::PMOS) return tech->spice["pmos"].asString();
        if (circuitType == ModuleType::NMOS) return tech->spice["nmos"].asString();

        if (circuits.empty())
            return std::string( CircuitFactory::modulesName[INT(circuitType)] );
        return util::format("%s_%d", 
                            CircuitFactory::modulesName[INT(circuitType)], 
                            circuits.size());
    }

    Circuit* CircuitFactory::createNewCircuit(ModuleType circuitType, 
                                CircuitArguments* arguments,
                                std::string circuitName) const
    {
        switch (circuitType)
        {
        case ModuleType::BITCELL:
            return Allocator::alloc<Bitcell>(std::move(circuitName), dynamic_cast<BitcellArguments*>(arguments));
        case ModuleType::BITCELL_ARRAY:
            return Allocator::alloc<BitcellArray>(std::move(circuitName), dynamic_cast<BitcellArrayArguments*>(arguments));
        case ModuleType::NMOS:
        case ModuleType::PMOS:
            return Allocator::alloc<MOS>(std::move(circuitName), dynamic_cast<MOSArguments*>(arguments));
        }
        return nullptr;
    }

    void CircuitFactory::collectCircuit(ModuleType circuitType, 
                                        std::string argsList,
                                        Circuit* circuit)
    {
        auto& circuits{ this->_circuits[INT(circuitType)] };
        circuits.emplace( std::move(argsList), circuit );
    }

    CircuitFactory::~CircuitFactory() noexcept
    {
        for (auto& circuits : this->_circuits)
        {
            for (auto& kv : circuits)
            {
                Allocator::free(kv.second);
                kv.second = nullptr;
            }
        }
    }
}