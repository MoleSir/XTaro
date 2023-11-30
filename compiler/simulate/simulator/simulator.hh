#pragma once

#include "../command/measurement.hh"

#include <string>
#include <fstream>
#include <vector>

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
                          const std::vector<double>& voltages);
        void addDCSupply(const std::string& supplyName,
                         const std::string& portName,
                         double voltage);
        void addTrans(double interval, double endTime);

        void run();

        void getResult(Measurement* measurement);

    private:
        bool checkWritable() const;

    private:
        std::string _simulationFilename;
        std::string _outputFilename;
        std::ofstream _simulationFile;
        std::ifstream _outputFile;
    };
};