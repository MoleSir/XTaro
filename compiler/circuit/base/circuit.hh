#pragma once

#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>

namespace xtaro::circuit
{
    struct CircuitArguments { };

    /*
        Create a new Circuit:
        - Add ports
        - Create Sub Circuits
        - Create and connect instance
    */
    class Circuit
    {
    public:
        Circuit(std::string name, const std::string& spicefile = "");
        virtual ~Circuit() noexcept;

    public:
        void addPort(std::string portName, PortType portType);
        void addPorts(std::vector<std::string> portsName, PortType portType);

        Instance* addInstance(std::string instanceName, Circuit* circuit);
        std::vector<const char*> portsName() const;
        void connectWith(Instance* instance, const std::vector<std::string>& nets);
        std::vector<Net*> createNets(const std::vector<std::string>& netsName);
        std::vector<Port*> copyPort() const;
        void writeSpice(const std::string& filename);
        void createNetlist();

    protected:
        void doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited);

        virtual void createPorts() = 0;
        virtual void createCircuits() = 0;
        virtual void createInstances() = 0;

    public:
        const std::string& name() const noexcept
        { return this->_name; }
        
        const std::vector<Port*>& ports() const noexcept
        { return this->_ports; } 

        const std::set<Circuit*>& circuits() const noexcept
        { return this->_circuits; }

        const std::vector<Instance*>& instances() const noexcept
        { return this->_instances; }

    protected:
        std::string _name;
        std::set<Circuit*> _circuits;
        std::vector<Instance*> _instances;
        std::vector<Port*> _ports;
        std::map<std::string, Net*> _nets;

        bool _isMetaCircuit;
        std::string _metaSpice;
    };

}