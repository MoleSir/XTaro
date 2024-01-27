#pragma once

#include <stringpool/string.hh>

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
        Port(String name, PortType type);

    public:
        const String& name() const noexcept
        { return this->_name; }

        PortType type() const noexcept
        { return this->_type; }

        Net* net() const noexcept
        { return this->_net; }
        
    public:
        void setNet(Net* net) noexcept
        { this->_net = net; } 

    private:
        String _name;
        PortType _type;
        Net* _net;
    };

}