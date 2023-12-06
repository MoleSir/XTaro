#pragma once

#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{
    class Port;

    class Net
    {
    public:
        Net();

        const std::list<Port*> ports() const noexcept
        { return this->_ports; }
        
    private:
        std::list<Port*> _ports;    
    };

}