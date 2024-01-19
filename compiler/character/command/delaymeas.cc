#include "delaymeas.hh"
#include "global.hh"

namespace xtaro::character
{
    #define INT(e) static_cast<int>(e)

    std::array<const char*, EDGE_DIRECTION_SIZE> 
    DelayMeasurement::edgeDirectionNames 
    {
        "FALL", "RISE"
    };

    DelayMeasurement::DelayMeasurement(
            std::string name,
            std::string trigNetName, EdgeDirection trigDirection, double trigVoltage, double trigTimeDelay,
            std::string targNetName, EdgeDirection targDirection, double targVoltage, double targTimeDelay) :

        Measurement{std::move(name)},

        _trigNetName{std::move(trigNetName)}, 
        _trigDirection{trigDirection}, 
        _trigVoltage{trigVoltage}, 
        _trigTimeDalay{trigTimeDelay},
        
        _targNetName{std::move(targNetName)}, 
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
                
                this->_trigNetName.c_str(),
                this->_trigVoltage,
                DelayMeasurement::edgeDirectionNames[INT(this->_trigDirection)],
                this->_trigTimeDalay,
            
                this->_targNetName.c_str(),
                this->_targVoltage,
                DelayMeasurement::edgeDirectionNames[INT(this->_targDirection)],
                this->_targTimeDalay
            );
        file << command;
    }

}