#include "circuitfactory.hh"
#include <allocator/allocator.hh>

#include <base/circuit.hh>
#include <module/bitcell.hh>
#include <module/bitcellarray.hh>

namespace xtaro::circuit
{
    CircuitFactory* factory{CircuitFactory::instance()};

    Circuit* CircuitFactory::create(CircuitType circuitType, std::string circuitName, CircuitArguments* arguments)
    {
        Circuit* circuit{this->findCircuit(circuitName)};
        if (circuit != nullptr)
            return circuit;

        switch (circuitType) 
        {
        case CircuitType::BITCELL:
            circuit = Allocator::alloc<Bitcell>(circuitName, static_cast<BitcellArguments*>(arguments)); break;
        case CircuitType::BITCELL_ARRAY:
            circuit = Allocator::alloc<BitcellArray>(circuitName, static_cast<BitcellArrayArguments*>(arguments)); break;
        default: 
            circuit = nullptr; break;
        }   

        this->_circuits.emplace(std::move(circuitName), circuit);
        return circuit;
    }

    CircuitFactory* CircuitFactory::instance()
    {
        static CircuitFactory _factory;
        return &_factory;
    }

    Circuit* CircuitFactory::findCircuit(const std::string& circuitName) const
    {   
        auto iter{ this->_circuits.find(circuitName) };
        if (iter != this->_circuits.end())
            return iter->second;
        return nullptr;
    }

    CircuitFactory::~CircuitFactory() noexcept
    {
        for (auto& kv : this->_circuits)
        {
            Allocator::free(kv.second);
            kv.second = nullptr;
        }
    }
}