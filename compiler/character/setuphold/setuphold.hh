#pragma once

#include <string>

namespace xtaro::simulate
{
    class Simulator;
}

namespace xtaro::character
{

    class SetupHoldCharacterizer
    {
        enum EdgeDirection
        {
            FALL = 0, RISE = 1,
        };

        enum TimeMode
        {
            SETUP = 0, HOLD = 1,
        };

    public:
        SetupHoldCharacterizer(std::string dffFilename, double period) noexcept;

        double getTime(EdgeDirection direction, TimeMode mode);

        double getSetupLHTime()
        { return this->getTime(EdgeDirection::RISE, TimeMode::SETUP); }
        
        double getSetupHLTime()
        { return this->getTime(EdgeDirection::FALL, TimeMode::SETUP); }
        
        double getHoldLHTime()
        { return this->getTime(EdgeDirection::RISE, TimeMode::HOLD); }

        double getHoldHLTime()
        { return this->getTime(EdgeDirection::FALL, TimeMode::HOLD); }

    private:
        double simulate(EdgeDirection direction, TimeMode mode, double targetTime, bool& success);
        void writeSimulateHeader();
        void writeSimulatInstance();
        void writeSimulateData(EdgeDirection direction, TimeMode mode, double targetTime);
        void writeSimulateClock();
        void writeSimulatControl();

    private:
        std::string _dffFilename;
        double _period;
        simulate::Simulator* _simulator;

        double _relatedInputSlew;
        double _constrainedInputSlew;
    };

}