#pragma once

#include <base/circuit.hh>

#include <string>
#include <map>

namespace xtaro::circuit
{
    enum class CircuitType
    {
        BITCELL,
        BITCELL_ARRAY,
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
        Circuit* create(CircuitType circuitType, 
                        std::string circuitName, 
                        CircuitArguments* arguments = nullptr);

    private:
        Circuit* findCircuit(const std::string& circuitName) const;

    private:
        std::map<std::string, Circuit*> _circuits{};
    };

    extern CircuitFactory* factory;
}