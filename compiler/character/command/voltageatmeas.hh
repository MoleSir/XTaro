#pragma once

#include "meas.hh"

#include <string>
#include <fstream>

namespace xtaro::character
{

    // .meas tran vdout0_0ck40 FIND v(dout0_0) AT=50.05n 
    class VoltageAtMeasurement : public Measurement
    {
    public:

    public:
        VoltageAtMeasurement(std::string name, std::string netName, float measureTime);
        virtual void writeCommand(std::ofstream& file) const override;

    private:
        std::string _netName;
        float _measureTime;
    };

}