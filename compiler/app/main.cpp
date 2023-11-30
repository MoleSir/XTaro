#include <iostream>
#include "../global/global.hh"
#include "../simulate/simulator/simulator.hh"
#include "../simulate/command/delaymeasurement.hh"

using namespace xtaro;

int main()
{
    logger->open("/mnt/e/XTaro/temp/xtrao.log");

    simulate::Simulator simulator("/mnt/e/XTaro/temp/sim.sp", "/mnt/e/XTaro/temp/timing.lis");
    
    simulator.addInclude("/mnt/e/XTaro/temp/nmos.sp");    
    simulator.addInclude("/mnt/e/XTaro/temp/pmos.sp");
    simulator.addInclude("/mnt/e/XTaro/temp/dff.sp");

    simulator.addInstance("dff", {"D", "Q", "clk", "vdd", "0"}, "dff");
    simulator.addDCSupply("vdd", "vdd", 5.0);
    simulator.addPWLSupply("clk", "clk", 
                          {0.0, 1.0, 3.0, 9.0, 11.0, 19.0, 21.0},
                          {0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 5.0});
    simulator.addPWLSupply("D", "D", 
                          {0.0, 14.0, 16.0},
                          {0.0, 0.0, 5.0,});
    
    simulate::DelayMeasurement* m = new simulate::DelayMeasurement(
            "delay",
            "clk", simulate::DelayMeasurement::EdgeDirection::RISE, 2.5, 15.0,
            "Q", simulate::DelayMeasurement::EdgeDirection::RISE, 2.5, 15.0
    );
    simulator.addMeasurement(m);

    simulator.addTrans(10, 40);
    simulator.addTemperature(25);
    simulator.addEnd();

    simulator.run();
    simulator.getResult(m);

    return 0;
}