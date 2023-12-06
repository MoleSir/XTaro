#pragma once

#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{
    class Net;
    class Circuit;
    class Instance;

    enum class PortType
    {
        INPUT, OUTPUT, INOUT,
    };

    class Port
    {
    public:
        Port(std::string name, PortType type);

    public:
        const std::string& name() const noexcept
        { return this->_name; }

        PortType type() const noexcept
        { return this->_type; }

        Net* net() const noexcept
        { return this->_net; }

        Circuit* circuit() const noexcept
        { return this->_circuit; }

    public:
        void setNet(Net* net) noexcept
        { this->_net = net; } 

        void setCircuit(Circuit* circuit) noexcept
        { this->_circuit = circuit; }

        void setInstance(Instance* instance) noexcept
        { this->_instance = instance; }

    private:
        std::string _name;
        PortType _type;
        Net* _net;

        union
        {
            Circuit* _circuit;
            Instance* _instance;
        };
    };

}