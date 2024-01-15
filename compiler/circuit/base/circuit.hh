#pragma once

#include "circuitenum.hh"
#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include <stringpool/string.hh>

#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>

namespace xtaro::circuit
{
    struct CircuitArguments 
    {
        virtual ~CircuitArguments() noexcept = default;
        virtual std::string toString() const = 0; 
    };
    
    /*
        Create a new Circuit:
        - Add ports
        - Create Sub Circuits
        - Create and connect instance
    */

    class Circuit
    {
    public:
        Circuit(String name, DeviceType type, const std::string& spicefile = "");
        virtual ~Circuit() noexcept;

    public:
        void addPort(String portName, PortType portType);
        void addPorts(const std::vector<String>& portsName, PortType portType);
        std::vector<const char*> portsName() const;
        std::vector<Port*> copyPort() const;

        Instance* addInstance(String instanceName, Circuit* circuit);
        void connectWith(Instance* instance, const std::vector<String>& nets);

        void writeSpice(const std::string& filename);
        void createNetlist();

    protected:
        std::vector<Net*> createNets(const std::vector<String>& netsName);

    protected:
        void doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited);

        virtual void createPorts() = 0;
        virtual void createCircuits() = 0;
        virtual void createInstances() = 0;
        
    public:
        virtual std::string argumentsList() const 
        { return ""; }

    public:
        const String& name() const noexcept
        { return this->_name; }

        DeviceType type() const noexcept
        { return this->_type; }
        
        const std::vector<Port*>& ports() const noexcept
        { return this->_ports; } 

        const std::set<Circuit*>& circuits() const noexcept
        { return this->_circuits; }

        const std::vector<Instance*>& instances() const noexcept
        { return this->_instances; }

    protected:
        String _name;
        DeviceType  _type;

        std::set<Circuit*> _circuits;
        std::vector<Instance*> _instances;
        std::vector<Port*> _ports;
        std::map<String, Net*> _nets;

        bool _isMetaCircuit;
        std::string _metaSpice;
    };

}