#include "voltagemeasurement.hh"
#include "global.hh"

#include <string>
#include <fstream>

namespace xtaro::simulate
{
    VoltageAtMeasurement::VoltageAtMeasurement(
        std::string name, std::string measureName, float measureTime):
        Measurement{std::move(name)},
        _measureName{std::move(measureName)},
        _measureTime{measureTime}
    {
    }

    void VoltageAtMeasurement::writeCommand(std::ofstream& file) const
    {
        std::string command = util::format(
            ".meas tran %s FIND v(%s) AT=%fn\n",
            this->_name.c_str(),
            this->_measureName.c_str(),
            this->_measureTime
        );

        file << command;
    }
}