#include "circuitfactory.hh"
#include <allocator/allocator.hh>

namespace xtaro::circuit
{
    CircuitFactory* factory{CircuitFactory::instance()};

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