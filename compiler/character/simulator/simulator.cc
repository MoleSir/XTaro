#include "simulator.hh"

#include <command/meas.hh>

#include <util/util.hh>
#include <log/logger.hh>
#include <exception/msgexception.hh>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

namespace xtaro::character
{
    Simulator::Simulator(std::string simulationFilename): 
        _simulationFilename{std::move(simulationFilename)},
        _simulationFile{}
    {
        this->_simulationFile.open(this->_simulationFilename);
        if (!this->_simulationFile.is_open())
        {
            std::string errorMsg {
                util::format("Open '%d' failed!", this->_simulationFilename.c_str())
            };

            logger->error(errorMsg);
            throw MessageException("Generate spice simulate file", errorMsg);
        }
    }

    Simulator::~Simulator() noexcept = default;

    void Simulator::writeContent(const std::string& content)
    {
        this->checkFileWritable();
        this->_simulationFile << content;
    }
    
    void Simulator::writeContent(char ch)
    {
        this->checkFileWritable();
        this->_simulationFile << ch;
    }

    void Simulator::writeInclude(const std::string& filename)
    {
        this->checkFileWritable();

        std::string command = util::format(".include \"%s\"\n", filename.c_str());
        this->_simulationFile << command;
    }

    void Simulator::writeMeasurement(Measurement* measurement)
    {
        this->_measurements.emplace_back(measurement);
        measurement->writeCommand(this->_simulationFile);
    }

    void Simulator::writeEnd()
    {
        this->checkFileWritable();
        this->_simulationFile << ".end\n";
    }

    void Simulator::writeComment(const std::string& comment)
    {
        this->checkFileWritable();
        this->_simulationFile << "* " << comment << '\n';
    }

    void Simulator::writeTemperature(double temperature)
    {
        this->checkFileWritable();

        this->_simulationFile << util::format(".TEMP %f\n", temperature);
    }

    void Simulator::writeInstance(
                    const std::string& moduleName,
                    const std::string& instanceName, 
                    const std::vector<std::string>& portsName)
    {
        this->checkFileWritable();

        this->_simulationFile << 'X' <<  instanceName;
        
        for (const std::string& portName : portsName)
            this->_simulationFile << ' ' << portName;
        
        this->_simulationFile << ' ' << moduleName << '\n';
    }

    void Simulator::writePWLVoltage(
                    const std::string& supplyName, 
                    const std::string& portName, 
                    const std::vector<double>& times,
                    const std::vector<double>& voltages,
                    double slew)
    {
        this->checkFileWritable();

        if (times.size() != voltages.size())
        {
            std::string errorMsg {
                util::format("Time size '%d' != Voltage size '%d'", times.size(), voltages.size())
            };
            
            logger->error(errorMsg);
            throw MessageException("Write PWL volatage", errorMsg);
        }

        // Vclk clk 0 PWL (0n 0.0v 1n 0.0v 3n 5v 9n 5V 11n 0v 19n 0v 21n 5v) 
        this->_simulationFile << util::format("V%s %s 0 PWL (", supplyName.c_str(), portName.c_str());
        
        if (times.size() == 0)
        {
            this->_simulationFile << ")\n";
            return;
        }

        this->_simulationFile << util::format("%fn %fv ", times[0], voltages[0]);

        for (std::size_t i = 1; i < times.size(); ++i)
        {
            this->_simulationFile << util::format("%fn %fv ", times[i] - slew, voltages[i - 1]);
            this->_simulationFile << util::format("%fn %fv ", times[i] + slew, voltages[i]);
        }

        this->_simulationFile << ")\n";
    }


    void Simulator::writePWLVoltage(
                    const std::string& supplyName, 
                    const std::string& portName, 
                    const std::vector<double>& times,
                    const std::vector<double>& voltages)
    {
        this->checkFileWritable();

        if (times.size() != voltages.size())
        {
            std::string errorMsg {
                util::format("Time size '%d' != Voltage size '%d'", times.size(), voltages.size())
            };

            logger->error(errorMsg);
            throw MessageException("Write PWL volatage", errorMsg);
        }

        // Vclk clk 0 PWL (0n 0.0v 1n 0.0v 3n 5v 9n 5V 11n 0v 19n 0v 21n 5v) 
        this->_simulationFile << util::format("V%s %s 0 PWL (", supplyName.c_str(), portName.c_str());

        for (std::size_t i = 0; i < times.size(); ++i)
            this->_simulationFile << util::format("%fn %fv ", times[i], voltages[i]);

        this->_simulationFile << ")\n";
    }

    void Simulator::writePULSEVoltage(
                    const std::string& volatgeName,
                    const std::string& netName, 
                    double initVoltage,
                    double pulsedVoltage,
                    double delayTime,
                    double riseTime,
                    double fallTime,
                    double pulseWidth,
                    double period)
    {
        this->checkFileWritable();

        // Vclk0 clk0 0 PULSE (0 3.3 9.95n 0.1n 0.1n 4.9n 10n)
        this->_simulationFile << util::format(
            "V%s %s 0 PULSE(%f %f %fn %fn %fn %fn %fn)\n",
            volatgeName.c_str(), netName.c_str(),
            initVoltage, pulseWidth, 
            delayTime,
            riseTime, fallTime,
            pulseWidth, period
        );
    }

    void Simulator::writeDCVoltage(
                    const std::string& supplyName,
                    const std::string& portName,
                    double voltage)
    {
        this->checkFileWritable();

        this->_simulationFile << util::format(
            "V%s %s 0 %f\n", 
            supplyName.c_str(), 
            portName.c_str(), 
            voltage
        );
    }

    void Simulator::writeTrans(double stepTime, double starTime, double endTime)
    {
        this->checkFileWritable();

        this->_simulationFile << util::format(
            ".TRAN %fp %fn %fn\n", 
            stepTime, 
            endTime,
            starTime
        );
    }

    void Simulator::writeCapacitance(const std::string& name, 
                                   const std::string& portName1, 
                                   const std::string& protName2,
                                   double value)
    {
        this->checkFileWritable();
        
        // CD00 dout0_0 0 0f
        this->_simulationFile << util::format(
            "C%s %s %s %ff", 
            name.c_str(), 
            portName1.c_str(), 
            protName2.c_str(), 
            value
        );
    }
    
    std::map<std::string, double> 
    Simulator::run(const std::string& outputFilename)
    {
        this->executeCommand(outputFilename);
        return this->getResults(outputFilename);
    }

    void Simulator::executeCommand(const std::string& outputFilename)
    {
        //!\note Close simulate file!!!
        this->_simulationFile.close();

        // Execute spice 
        std::string bashCommand {util::format(
            "/bin/bash -c 'ngspice -b -o %s %s'", 
            outputFilename.c_str(), 
            this->_simulationFilename.c_str()            
        )};
        util::execute(bashCommand);
    }

    std::map<std::string, double> 
    Simulator::getResults(const std::string& outputFilename)
    {
        // Read output file content
        std::ifstream infile {outputFilename};
        std::string content {util::readFile(infile)};

        // Get each result
        std::map<std::string, double> results {};
        for (auto& meas : this->_measurements)
        {
            // TODO: When failed...
            if (meas->getMeasureResult(content))
                results.emplace(meas->name(), meas->result());
        }

        return results;
    }

    void Simulator::checkFileWritable() const
    {
        if (!this->_simulationFile.is_open())
        {
            std::string errorMsg {"The simulation file already closed."};

            logger->error(errorMsg);
            throw MessageException("Write spice simulation", errorMsg);
        }
    }

}