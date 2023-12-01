#pragma once

#include "measurement.hh"

#include <string>
#include <fstream>

namespace xtaro::simulate
{

    enum EdgeDirection
    {
        FALL = 0,
        RISE,
        COUNT,
    };

    class DelayMeasurement : public Measurement
    {
    public:
        DelayMeasurement(std::string name,
                         std::string trigName, EdgeDirection trigDirection, float trigVoltage, float trigTimeDelay,
                         std::string targName, EdgeDirection targDirection, float targVoltage, float targTimeDelay);

        virtual void writeCommand(std::ofstream& file) const override;

    private:
        std::string _trigName;
        EdgeDirection _trigDirection;
        float _trigVoltage;
        float _trigTimeDalay;

        std::string _targName;
        EdgeDirection _targDirection;
        float _targVoltage;
        float _targTimeDalay;

    private:
        static const char* _edgeDirectionNames[EdgeDirection::COUNT];
    };

}