#pragma once

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

}