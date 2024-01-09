#pragma once

#include "./measurement.hh"

#include <string>
#include <fstream>
#include <vector>
#include <list>

namespace xtaro::simulate
{

    class Simulator
    {
    public:
        Simulator(std::string simulationFilename, std::string outputFilename);

        void addInclude(const std::string& filename);
        void addMeasurement(Measurement* measurement);
        void addEnd();
        void addTemperature(double temperature);
        void addInstance(const std::string& instanceName, 
                         const std::vector<std::string>& portsName,
                         const std::string& moduleName);
        void addPWLSupply(const std::string& supplyName, 
                          const std::string& portName, 
                          const std::vector<double>& times,
                          const std::vector<double>& voltages,
                          double slew = 0.0);
        void addDCSupply(const std::string& supplyName,
                         const std::string& portName,
                         double voltage);
        void addTrans(double interval, double endTime);
        void addCapacitance(const std::string& name, 
                            const std::string& portName1, 
                            const std::string& protName2,
                            double value);

        void run();

        bool getResult(Measurement* measurement);

    private:
        bool checkWritable() const;

    private:
        std::string _simulationFilename;
        std::string _outputFilename;
        std::ofstream _simulationFile;
        std::ifstream _outputFile;
    };
};