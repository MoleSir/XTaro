#pragma once

#include "circuittype.hh"

#include <stringpool/string.hh>
#include <base/circuit.hh>

#include <string>
#include <map>

namespace xtaro::circuit
{
    
    class CircuitFactory
    {
        using CircuitMap = std::map<std::string, Circuit*>;
        static const std::array<const char*, CIRCUIT_TYPE_SIZE> modulesName;

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
                        CircuitArguments* arguments,
                        String circuitName = "");

    private:
        Circuit* findCircuit(CircuitType circuitType, const std::string& argsList) const;
        String getDefaultCircuitName(CircuitType circuitType) const;
        Circuit* createNewCircuit(CircuitType circuitType, 
                                  CircuitArguments* arguments,
                                  String circuitName) const;
        void collectCircuit(CircuitType circuitType, std::string argsList, Circuit* circuit);

    private:
        std::array<CircuitMap, CIRCUIT_TYPE_SIZE> _circuits{};
    };

    extern CircuitFactory* factory;
}