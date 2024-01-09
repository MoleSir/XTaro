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
        
    public:
        void setNet(Net* net) noexcept
        { this->_net = net; } 

    private:
        std::string _name;
        PortType _type;
        Net* _net;
    };

}