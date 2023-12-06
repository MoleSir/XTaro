#pragma once

#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>

#include "instance.hh"
#include "net.hh"
#include "port.hh"

namespace xtaro::circuit
{
    class Circuit
    {
    public:
        Circuit(std::string name);
        ~Circuit() noexcept;

    public:
        void addPort(std::string name, PortType type);
        std::vector<const char*> portsName() const;

    public:
        const std::string& name() const noexcept
        { return this->_name; }
        
        const std::map<std::string, Port*>& ports() const noexcept
        { return this->_ports; } 

        const std::set<Circuit*>& circuits() const noexcept
        { return this->_circuits; }

        const std::list<Instance*>& instances() const noexcept
        { return this->_instances; }

    private:
        std::string _name;
        std::map<std::string, Port*> _ports;
        std::set<Circuit*> _circuits;
        std::list<Instance*> _instances;
    };

}