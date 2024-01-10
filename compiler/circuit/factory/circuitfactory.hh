#pragma once

#include <base/circuit.hh>
#include <module/bitcell.hh>
#include <allocator/allocator.hh>

#include <string>
#include <map>

namespace xtaro::circuit
{
    enum class CircuitType
    {
        BITCELL,
    };
    
    class CircuitFactory
    {
    private:
        CircuitFactory() = default;
        ~CircuitFactory() noexcept;
        CircuitFactory(const CircuitFactory&) = delete;
        CircuitFactory(CircuitFactory&&) = delete;
        CircuitFactory& operator = (const CircuitFactory&) = delete;
        CircuitFactory& operator = (CircuitFactory&&) = delete;

    public:
        static CircuitFactory* instance();

    public:
        template<typename... Args>
        Circuit* create(CircuitType circuitType, std::string circuitName, Args... args)
        {
            Circuit* circuit{this->findCircuit(circuitName)};
            if (circuit != nullptr)
                return circuit;

            switch (circuitType) 
            {
            case CircuitType::BITCELL:
                circuit = Allocator::alloc<Bitcell>(circuitName); break;
            default: circuit = nullptr;
            }   

            this->_circuits.emplace(std::move(circuitName), circuit);
            return circuit;
        }

    private:
        Circuit* findCircuit(const std::string& circuitName) const;

    private:
        std::map<std::string, Circuit*> _circuits{};
    };

    extern CircuitFactory* factory;
}