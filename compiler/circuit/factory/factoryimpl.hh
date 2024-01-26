#pragma once

#include "factory.hh"
#include "circuittype.hh"

#include <string>
#include <memory>
#include <unordered_map>
#include <string_view>
#include <array>

namespace xtaro::circuit
{

    class CircuitFactory::Impl
    {
        using CircuitMap = std::unordered_map<std::string, Circuit*>;
        static std::unordered_map<std::string_view, CircuitType> circuitTypes;

    public:
        Impl() = default;
        ~Impl() noexcept;
        Impl(const Impl&) = delete;
        Impl(Impl&&) = delete;
        Impl& operator = (const Impl&) = delete;
        Impl& operator = (Impl&&) = delete;

    public:
        void reset() noexcept;

        Circuit* create(const std::string_view& circuitTypeName,
                        CircuitArguments* arguments,
                        String circuitName);

    private:
        static CircuitType getCircuitType(const std::string_view& circuitType);

        Circuit* findCircuit(CircuitType circuitType, const std::string& argsList) const;
        
        String getDefaultCircuitName(const std::string_view& circuitTypeName,
                                     CircuitType circuitType) const;
        
        Circuit* createNewCircuit(CircuitType circuitType, 
                                  CircuitArguments* arguments,
                                  String circuitName) const;
        
        void collectCircuit(CircuitType circuitType, std::string argsList, Circuit* circuit);

    private:
        std::array<CircuitMap, CIRCUIT_TYPE_SIZE> _circuits;
    };

    extern CircuitFactory* factory;
}