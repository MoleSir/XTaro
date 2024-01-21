#include "voltageatmeas.hh"
#include <util/format.hh>

#include <string>
#include <fstream>
#include <regex>

namespace xtaro::character
{
    VoltageAtMeasurement::VoltageAtMeasurement(
        std::string name, std::string netName, double measureTime):
        Measurement{std::move(name)},
        _netName{std::move(netName)},
        _measureTime{measureTime}
    {
    }

    void VoltageAtMeasurement::writeCommand(std::ofstream& file) const
    {
        std::string command = util::format(
            ".meas tran %s FIND v(%s) AT=%fn\n",
            this->_name.c_str(),
            this->_netName.c_str(),
            this->_measureTime
        );

        file << command;
    }
    
}