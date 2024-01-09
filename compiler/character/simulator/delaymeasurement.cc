#include "delaymeasurement.hh"
#include "global.hh"

namespace xtaro::simulate
{
    const char* DelayMeasurement::_edgeDirectionNames[EdgeDirection::COUNT] = 
    {
        "FALL", "RISE"
    };

    DelayMeasurement::DelayMeasurement(
            std::string name,
            std::string trigName, EdgeDirection trigDirection, float trigVoltage, float trigTimeDelay,
            std::string targName, EdgeDirection targDirection, float targVoltage, float targTimeDelay) :

        Measurement{std::move(name)},

        _trigName{std::move(trigName)}, 
        _trigDirection{trigDirection}, 
        _trigVoltage{trigVoltage}, 
        _trigTimeDalay{trigTimeDelay},
        
        _targName{std::move(targName)}, 
        _targDirection{targDirection}, 
        _targVoltage{targVoltage}, 
        _targTimeDalay{targTimeDelay}
    {
    }   
    
    // .meas tran delay_lh0 TRIG v(clk0) VAL=2.5 FALL=1 TD=0n TARG v(dout0_1) VAL=2.5 RISE=1 TD=0n

    void DelayMeasurement::writeCommand(std::ofstream& file) const
    {
        std::string command = 
            util::format(
                ".meas tran %s TRIG v(%s) VAL=%f %s=1 TD=%fn TARG v(%s) VAL=%f %s=1 TD=%fn\n",
                
                this->_name.c_str(),
                
                this->_trigName.c_str(),
                this->_trigVoltage,
                DelayMeasurement::_edgeDirectionNames[this->_trigDirection],
                this->_trigTimeDalay,
            
                this->_targName.c_str(),
                this->_targVoltage,
                DelayMeasurement::_edgeDirectionNames[this->_targDirection],
                this->_targTimeDalay
            );
        file << command;
    }

}