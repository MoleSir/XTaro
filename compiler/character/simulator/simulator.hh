#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <map>

namespace xtaro::character
{
    class Measurement;

    /*
        Spice simulator class.
    */
    class Simulator
    {
    public:
        /*
            Simulator's contruction.
            - simulationFilename: The file where the spice commands are generated;
        */
        Simulator(std::string simulationFilename);
        ~Simulator() noexcept;

        Simulator(const Simulator&) = delete;
        Simulator(Simulator&&) = delete;
        Simulator& operator = (const Simulator&) = delete;
        Simulator& operator = (Simulator&&) = delete;

    public:
        /*
            Write string to spice file
        */
        void writeContent(const std::string& content);
        void writeContent(char ch);
        
        /*
            Write a '.include' to command file. 
            - filename: The file need to include.
        */
        void writeInclude(const std::string& filename);

        /*
            Write a '.END' spice command.
        */
        void writeEnd();

        /*
            Write spice 
        */
        void writeComment(const std::string& comment);
        
        /*
            Write a '.TEMP' command.
            - temperature: The temperature value.
        */
        void writeTemperature(double temperature);

        /*
            Write a SUBCKT instance.

            e.g, write an INV Gate, whose name is "inv0", and it connects with "in", "in_bar", "vdd" and "gnd".
            Jsut call: `writeInstance("inv", "inv0", {"in", "in_bar", "vdd", "gnd"});`.
            The nets in vector connect with ports vector of module by order.

            Method params:
            - moduleName: The module name of the instance belongs to. e.g "SRAM";
            - instanceName: The name of this instance. e.g "sram_16_2";
            - netsName: The name of the nets which connect with the ports of this instance.
              The net connects to port by order.
        */
        void writeInstance(
             const std::string& moduleName,
             const std::string& instanceName, 
             const std::vector<std::string>& netsName
        );

        /*
            Write a PWL Voltage source. 

            e.g: `writePWLVoltage("VCLK", "clk", {0, 1, 2}, {0, 3.3, 0}), 0.1);`
            Generate spice: VVCLK clk 0 PWL (0n 0.0v 0.95n 0v 1.05n 3.3v 1.95n 3.3v 2.05n 0v)

            Method params:
            - volatgeName: The name of this voltage source.
            - netName: The net which voltage source connect to.
            - times: The time sequence of PWL.
            - voltages: The voltage sequence of PWL.
            - slew: The slew of the Voltage.
        */
        void writePWLVoltage(
             const std::string& volatgeName, 
             const std::string& netName, 
             const std::vector<double>& times,
             const std::vector<double>& voltages,
             double slew
        );

        /*
            Write a PWL Voltage source. 
            But in this method, times and voltages write into spice directly.
        */
        void writePWLVoltage(
             const std::string& volatgeName, 
             const std::string& netName, 
             const std::vector<double>& times,
             const std::vector<double>& voltages
        );

        /*
            Write a PULSE Voltage source.
            - initVoltage: Initial value
            - pulsedVoltage: Pulsed value
            - delayTime: Delay time
            - riseTime: Rise time
            - fallTime: Fall time
            - pulseWidth: Pulse width
            - period: PER Period
        */
        void writePULSEVoltage(
             const std::string& volatgeName,
             const std::string& netName, 
             double initVoltage,
             double pulsedVoltage,
             double delayTime,
             double riseTime,
             double fallTime,
             double pulseWidth,
             double period 
        );

        /*
            Write a DC Voltage source.

            Method params:
            - volatgeName: The name of this voltage source.
            - netName: The net which voltage source connect to.
            - voltage: DC voltage value.
        */
        void writeDCVoltage(
             const std::string& volatgeName,
             const std::string& netName,
             double voltage);
        
        /*
            Write a '.TRAN' command.

            .TRAN {stepTime}p {endTime}n {startTIme}n {stepTime}p
            
            Method params:
            - stepTime: Interval time of simulation.
            - beginTime: The time which begins to collect data.
            - endTime: Simulation ends time.
        */
        void writeTrans(double stepTime, double beginTime, double endTime);

        /*
            Write a capacitance instance.

            C{capName} {net1Name} {net2Name} {capValue}f

            Method params:
            - capName: Name of capacitance.
            - net1Name: The name of net which connects which one side of CAP. 
            - net2Name: The name of net which connects which another side of CAP. 
            - value: Capacitance value.
        */
        void writeCapacitance(
             const std::string& capName, 
             const std::string& net1Name, 
             const std::string& net2tName,
             double capValue
        );

        /*
            Write spice measurement.
        */
        void writeMeasurement(Measurement* measurement);

    public:
        /*
            Run spice simulation. And return the result.
            - outputFilename: The file where the simulate report is generated.
        */
        std::map<std::string, double> run(const std::string& outputFilename);

    protected:
        /*
            Check the '_simulationFile' is writable.
            If it close, throw exception.
        */
        void checkFileWritable() const;

        void executeCommand(const std::string& outputFilename); 

        std::map<std::string, double> getResults(const std::string& outputFilename);

    protected:
        std::string _simulationFilename;
        std::ofstream _simulationFile;

        std::vector<std::unique_ptr<Measurement>> _measurements;
    };
};