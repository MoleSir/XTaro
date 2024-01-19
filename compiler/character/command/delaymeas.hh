#pragma once

#include "meas.hh"

#include <string>
#include <fstream>
#include <array>

namespace xtaro::character
{

    enum class EdgeDirection
    {
        FALL = 0,
        RISE,
        SIZE,
    };
    #define EDGE_DIRECTION_SIZE static_cast<int>(EdgeDirection::SIZE)

    class DelayMeasurement : public Measurement
    {
    public:
        DelayMeasurement(std::string name,
                         std::string trigNetName, EdgeDirection trigDirection, double trigVoltage, double trigTimeDelay,
                         std::string targNetName, EdgeDirection targDirection, double targVoltage, double targTimeDelay);

        virtual void writeCommand(std::ofstream& file) const override;

    private:
        std::string _trigNetName;
        EdgeDirection _trigDirection;
        double _trigVoltage;
        double _trigTimeDalay;

        std::string _targNetName;
        EdgeDirection _targDirection;
        double _targVoltage;
        double _targTimeDalay;

    private:
        static std::array<const char*, EDGE_DIRECTION_SIZE> edgeDirectionNames;
    };

}