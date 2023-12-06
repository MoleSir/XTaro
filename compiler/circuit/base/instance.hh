#pragma once

#include <string>
#include <map>
#include <list>
#include <string>

namespace xtaro::circuit
{
    class Circuit;
    class Port;

    class Instance
    {
    public:
        Instance(std::string name, Circuit* circuit);
        ~Instance() noexcept;

    public:
        const std::string& name() const noexcept
        { return this->_name; }

        Circuit* circuit() const noexcept
        { return this->_circuit; }

        const std::map<std::string, Port*> ports() const noexcept
        { return this->_ports; }

    private:
        std::string _name;
        Circuit* _circuit;
        std::map<std::string, Port*> _ports;
    };

}