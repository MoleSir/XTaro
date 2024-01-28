#pragma once

#include "circuitfactory.hh"

#include <string>
#include <unordered_map>
#include <string_view>
#include <array>

namespace xtaro::circuit
{

    enum class CircuitType
    { 
        BITCELL,
        DFF,
        PRECHARGE,
        SENSE_AMPLIFIER,
        TRISTATE_GATE,
        WRITE_DRIVER,
        BITCELL_ARRAY,
        MOS,
        INV,
        NAND,
        AND,
        NOR,
        OR,
        DECODER,
        ROW_DECODER,
        MUX,
        COLUMN_MUX,
        FANOUT_BUFFER,
        SENSE_AMPLIFIER_ARRAY,
        WRITE_DRIVER_ARRAY,
        PRECHARGE_ARRAY,
        REPLICA_BANK,
        BANK,
        CONTROL_LOGIC,
        SRAM,
        SIZE,
    };

    #define CIRCUIT_TYPE_SIZE static_cast<int>(CircuitType::SIZE)

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
                        std::string_view circuitName);

    private:
        static CircuitType getCircuitType(const std::string_view& circuitType);

        Circuit* findCircuit(CircuitType circuitType, const std::string& argsList) const;
        
        std::string_view getDefaultCircuitName(const std::string_view& circuitTypeName,
                                               CircuitType circuitType) const;
        
        Circuit* createNewCircuit(CircuitType circuitType, 
                                  CircuitArguments* arguments,
                                  const std::string_view& circuitName) const;
        
        void collectCircuit(CircuitType circuitType, std::string argsList, Circuit* circuit);

    private:
        std::array<CircuitMap, CIRCUIT_TYPE_SIZE> _circuits;
    };
    
}