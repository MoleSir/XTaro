#pragma once

#include <stringpool/string.hh>
#include <base/circuit.hh>

#include <string>
#include <map>

namespace xtaro::circuit
{
    enum class ModuleType
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
        SIZE,
    };

    #define MODULESIZE static_cast<int>(ModuleType::SIZE)
    
    class CircuitFactory
    {
        using CircuitMap = std::map<std::string, Circuit*>;
        static const std::array<const char*, MODULESIZE> modulesName;

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
        Circuit* create(ModuleType circuitType,
                        CircuitArguments* arguments,
                        String circuitName = "");

    private:
        Circuit* findCircuit(ModuleType circuitType, const std::string& argsList) const;
        String getDefaultCircuitName(ModuleType circuitType) const;
        Circuit* createNewCircuit(ModuleType circuitType, 
                                  CircuitArguments* arguments,
                                  String circuitName) const;
        void collectCircuit(ModuleType circuitType, std::string argsList, Circuit* circuit);

    private:
        std::array<CircuitMap, MODULESIZE>_circuits{};
    };

    extern CircuitFactory* factory;
}