#pragma once

namespace xtaro::circuit
{
    enum class DeviceType
    {
        MOS = 0,
        RESISTANCE,
        CAPACITANCE,
        DIODE,
        SUBCKT,
        SIZE,
    };
    #define DEVICE_SIZE static_cast<int>(DeviceType::SIZE)

    enum class DriveCapability
    {
        WEEK = 0,
        NORMAL,
        STRONG_1,
        STRONG_2,
        STRONG_3,
        SIZE,
    };

    enum class MOSType
    {
        NMOS, PMOS,
    };
}