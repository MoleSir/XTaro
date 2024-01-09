#pragma once

#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <fstream>

#include "instance.hh"
#include "net.hh"
#include "port.hh"

namespace xtaro::circuit
{
    class Instance;

    /*
        Create a new Circuit:
        - Add ports
        - Create Sub Circuits
        - Create and connect instance
    */
    class Circuit
    {
    public:
        Circuit(std::string name);
        ~Circuit() noexcept;

    public:
        void addPort(std::string name, PortType type);
        Instance* addInstance(std::string instanceName, Circuit* circuit);
        std::vector<const char*> portsName() const;
        void connectWith(Instance* instance, const std::vector<std::string>& nets);
        std::vector<Net*> createNets(const std::vector<std::string>& netsName);
        std::vector<Port*> copyPort() const;
        
        void createNetlist();

        void writeSpice(const std::string& filename);
        void doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited);

    private:
        virtual void createPorts() = 0;
        virtual void createSubckts() = 0;
        virtual void createInstances() = 0;

    public:
        const std::string& name() const noexcept
        { return this->_name; }
        
        const std::list<Port*>& ports() const noexcept
        { return this->_ports; } 

        const std::set<Circuit*>& circuits() const noexcept
        { return this->_circuits; }

        const std::list<Instance*>& instances() const noexcept
        { return this->_instances; }

    private:
        std::string _name;
        std::list<Port*> _ports;
        std::set<Circuit*> _circuits;
        std::list<Instance*> _instances;
        std::map<std::string, Net*> _nets;

        bool _isMetaCircuit;
        std::string _metaSpice;
    };

}