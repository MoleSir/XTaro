// #include "setuphold.hh"
// #include <simulator/simulator.hh>
// #include <simulator/delaymeas.hh>

// #include <log/log.hh>
// #include <util/util.hh>

// #include <string>
// #include <cmath>

// namespace xtaro::character
// {
//     SetupHoldCharacterizer::SetupHoldCharacterizer(std::string dffFilename, double period) noexcept:
//         _dffFilename{std::move(dffFilename)}, 
//         _period{period},
//         _simulator{nullptr},
//         _relatedInputSlew{0.4},
//         _constrainedInputSlew{0.4}
//     {}

//     double SetupHoldCharacterizer::getTime(EdgeDirection direction, TimeMode mode)
//     {
//         double feasibleBound, infeasibleBound;
//         if (mode == TimeMode::SETUP)
//         {
//             feasibleBound = 1.25 * this->_period;
//             infeasibleBound = 2.5 * this->_period;
//         }
//         else
//         {
//             infeasibleBound = 1.5 * this->_period;
//             feasibleBound = 2.75 * this->_period;
//         }
        
//         bool success = false;
//         double idealClkQ = 
//             this->simulate(direction, mode, feasibleBound, success);
//         if (success == false)
//             logger->error("Get setup/hold failed.");

//         double setupholdTime = (feasibleBound - 2 * this->_period);
//         double passingSetupHoldTime = 
//             mode == TimeMode::SETUP ? (-1 * setupholdTime) : setupholdTime;
        
//         logger->debug("Ideal time: %f", passingSetupHoldTime);

//         while (true)
//         {
//             double dataArriveTime = (feasibleBound + infeasibleBound) / 2.0;
//             logger->debug("Data arrive time: %f", dataArriveTime);
//             double clkToQ = this->simulate(direction, mode, dataArriveTime, success);

//             setupholdTime = (dataArriveTime - 2.0 * this->_period);
//             passingSetupHoldTime = 
//                 (mode == TimeMode::SETUP) ? (-1.0 * setupholdTime) : setupholdTime;            

//             if (success == true && (clkToQ < 1.1 * idealClkQ))
//             {
//                 logger->debug("PASS Clk-to-Q: %f Setup/Hold: %f", clkToQ, setupholdTime);
//                 feasibleBound = dataArriveTime;
//             }
//             else
//             {
//                 logger->debug("FAIL Clk-to-Q: %f Setup/Hold: %f", clkToQ, setupholdTime);
//                 infeasibleBound = dataArriveTime;
//             }

//             double maxBound = infeasibleBound > feasibleBound ? infeasibleBound : feasibleBound;
//             double relativeRatio = fabs(infeasibleBound - feasibleBound) / fabs(maxBound);
//             logger->debug("Relative ratio: %f", relativeRatio);
//             if (relativeRatio < 0.001)
//                 break;
//         };

//         return passingSetupHoldTime;
//     }

//     double SetupHoldCharacterizer::simulate(EdgeDirection direction, TimeMode mode, double dataArriveTime, bool& success)
//     {
//         Simulator simulator(
//             "/mnt/e/XTaro/temp/stim.sp", "/mnt/e/XTaro/temp/timing.lis");
//         this->_simulator = &simulator;

//         this->writeSimulateHeader();
//         this->writeSimulatInstance();
//         this->writeSimulateData(direction, mode, dataArriveTime);
//         this->writeSimulateClock();

//         DelayMeasurement m(
//             "clk2q_delay",
//             "clk", 
//             EdgeDirection::RISE, 
//             2.5, 
//             1.9 * this->_period,
//             "Q", 
//             direction == RISE ? EdgeDirection::RISE : EdgeDirection::FALL,
//             2.5, 
//             1.9 * this->_period
//         );
//         simulator.addMeasurement(&m);

//         this->writeSimulatControl();
        
//         simulator.run();
//         success = simulator.getResult(&m);
//         return m.result();
//     }

//     void SetupHoldCharacterizer::writeSimulateHeader()
//     {
//         this->_simulator->addInclude("/mnt/e/XTaro/temp/nmos.sp");    
//         this->_simulator->addInclude("/mnt/e/XTaro/temp/pmos.sp");
//         this->_simulator->addInclude("/mnt/e/XTaro/temp/dff.sp");
//         this->_simulator->addDCSupply("vdd", "vdd", 5.0);
//     }

//     void SetupHoldCharacterizer::writeSimulatInstance()
//     {
//         this->_simulator->addInstance("dff", {"D", "Q", "clk", "vdd", "0"}, "dff");
//     }

//     void SetupHoldCharacterizer::writeSimulateData(
//         EdgeDirection direction, TimeMode mode, double dataArriveTime)
//     {
//         // TODO
//         double correctValue   = direction == EdgeDirection::RISE ? 5.0 : 0.0;
//         double incorrectValue = direction == EdgeDirection::FALL ? 5.0 : 0.0; 
    
//         double initValue, startValue, endValue;
//         if (mode == TimeMode::SETUP) 
//         {
//             initValue = incorrectValue;
//             startValue = incorrectValue;
//             endValue = correctValue;
//         }
//         else
//         {
//             initValue = incorrectValue;
//             startValue = correctValue;
//             endValue = incorrectValue;
//         }

//         this->_simulator->addPWLSupply(
//             "D", "D",
//             {0, this->_period, dataArriveTime},
//             {initValue, startValue, endValue},
//             this->_constrainedInputSlew
//         );
//     }

//     void SetupHoldCharacterizer::writeSimulateClock()
//     {
//         this->_simulator->addPWLSupply(
//             "clk", "clk",
//             {0, 0.1 * this->_period, this->_period, 2 * this->_period},
//             {0, 5, 0, 5},
//             this->_relatedInputSlew
//         );
//     }

//     void SetupHoldCharacterizer::writeSimulatControl()
//     {
//         this->_simulator->addTrans(10, 4 * this->_period);
//         this->_simulator->addTemperature(25);
//         this->_simulator->addEnd();
//     }
// }