#include "circuitfactory.hh"
#include "circuitfactoryimpl.hh"

#include <base/circuit.hh>

#include <string>
#include <string_view>
#include <memory>

namespace xtaro::circuit
{

    CircuitFactory* circuitFactory {CircuitFactory::instance()};

    CircuitFactory::CircuitFactory() :
        _impl{std::make_unique<Impl>()}
    {
    }
    
    CircuitFactory::~CircuitFactory() noexcept = default;

    void CircuitFactory::reset() noexcept
    {
        this->_impl->reset();
    }

    Circuit* CircuitFactory::create(
             const std::string_view& circuitType,
             CircuitArguments* arguments,
             const std::string_view& circuitName)
    {
        return this->_impl->create(circuitType, arguments, circuitName);
    }

    CircuitFactory* CircuitFactory::instance()
    {
        static CircuitFactory _factory;
        return &_factory;
    }

}