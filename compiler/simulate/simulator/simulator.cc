#include "simulator.hh"
#include "./util/util.hh"
#include "./log/log.hh"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace xtaro::simulate
{
    Simulator::Simulator(std::string simulationFilename, std::string outputFilename): 
        _simulationFilename{std::move(simulationFilename)},
        _outputFilename{std::move(outputFilename)},
        _simulationFile{}, _outputFile{}
    {
        this->_simulationFile.open(this->_simulationFilename);
    }

    void Simulator::addInclude(const std::string& filename)
    {
        if (!this->checkWritable()) return;

        std::string command = util::format(".include \"%s\"\n", filename.c_str());
        this->_simulationFile << command;
    }

    void Simulator::addMeasurement(Measurement* measurement)
    {
        if (!this->checkWritable()) return;
        measurement->writeCommand(this->_simulationFile);
    }

    void Simulator::addEnd()
    {
        if (!this->checkWritable()) return;
        this->_simulationFile << ".end\n";
    }

    void Simulator::addTemperature(double temperature)
    {
        if (!this->checkWritable()) return;
        this->_simulationFile << util::format(".TEMP %f\n", temperature);
    }

    void Simulator::addInstance(const std::string& instanceName, 
                                const std::vector<std::string>& portsName,
                                const std::string& moduleName)
    {
        if (!this->checkWritable()) return;

        this->_simulationFile << 'X' <<  instanceName;
        
        for (const std::string& portName : portsName)
            this->_simulationFile << ' ' << portName;
        
        this->_simulationFile << ' ' << moduleName << '\n';
    }

    void Simulator::addPWLSupply(const std::string& supplyName, 
                                 const std::string& portName, 
                                 const std::vector<double>& times,
                                 const std::vector<double>& voltages)
    {
        if (!this->checkWritable()) return;

        if (times.size() != voltages.size())
        {
            logger->error(
                "Write PWL failed! times' size '%lu' is not equal to voltages' size '%lu'",
                times.size(), voltages.size()
            );
            return;
        }

        // Vclk clk 0 PWL (0n 0.0v 1n 0.0v 3n 5v 9n 5V 11n 0v 19n 0v 21n 5v) 
        this->_simulationFile << util::format("V%s %s 0 PWL (", supplyName.c_str(), portName.c_str());
        
        for (std::size_t i = 0; i < times.size(); ++i)
        {
            this->_simulationFile << util::format("%fn %fv", times[i], voltages[i]);
            if (i != times.size() - 1)
                this->_simulationFile << ' ';
            else
                this->_simulationFile << ')';
        }

        this->_simulationFile << '\n';
    }

    void Simulator::addDCSupply(const std::string& supplyName,
                                const std::string& portName,
                                double voltage)
    {
        if (!this->checkWritable()) return;
        this->_simulationFile << util::format("V%s %s 0 %f\n", supplyName.c_str(), portName.c_str(), voltage);
    }

    void Simulator::addTrans(double interval, double endTime)
    {
        if (!this->checkWritable()) return;
        this->_simulationFile << util::format(".TRAN %fp %fn 0n %fp UIC\n", interval, endTime, interval);
    }

    void Simulator::run()
    {
        this->_simulationFile.close();

        util::execute(util::format(
            "/bin/bash -c 'ngspice -b -o %s %s'", 
            this->_outputFilename.c_str(), 
            this->_simulationFilename.c_str()
        ));
    }

    void Simulator::getResult(Measurement* measurement)
    {
        // Read output file
        this->_outputFile.open(this->_outputFilename);
        this->_outputFile.seekg(0, std::ios::end);
        std::size_t size = this->_outputFile.tellg();
        this->_outputFile.seekg(0, std::ios::beg);

        std::string content(size + 1, '\0');
        this->_outputFile.read(content.data(), size);

        // Get result 
        if (measurement->getMeasureResult(content))
            std::cout << measurement->result() << '\n';
    }

    bool Simulator::checkWritable() const
    {
        if (this->_simulationFile.is_open() == false)
        {
            logger->error("Write .sp file failed! %s is not open", this->_simulationFilename.c_str());
            return -1;
        }
        return true;
    }
}