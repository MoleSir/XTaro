#include "instance.hh"
#include "net.hh"
#include "circuit.hh"
#include "port.hh"

#include <allocator/allocator.hh>
#include <util/util.hh>
#include <exception/msgexception.hh>

#include <string>
#include <map>
#include <list>
#include <string>
#include <sstream>

namespace xtaro::circuit
{
    #define INT(t) static_cast<int>(t)
    const std::array<char, DEVICE_SIZE> Instance::deviceKeywords {
        'M', 'R', 'C', 'D', 'X', 
    };

    Instance::Instance(std::string name, Circuit* circuit) :
        _name{std::move(name)},
        _circuit{circuit},
        _ports{std::move( circuit->copyPort() )},
        _connectCount{0} 
    {
    }

    Instance::~Instance() noexcept
    {
        for (Port* port : this->_ports)
            Allocator::free<Port>(port);
    }

    void Instance::connectNet(const std::string& portName, Net* net)
    {
        Port* port = this->port(portName);
        if (port != nullptr)
        {
            port->setNet(net);
            this->_connectCount += 1;
        }
    }

    void Instance::connectNets(const std::vector<Net*>& nets)
    {
        for (std::size_t i = 0; i < this->_ports.size(); ++i)
        {
            this->_ports[i]->setNet(nets[i]);
            this->_connectCount += 1;
        }
    }

    std::string Instance::spiceCommand() const
    {
        if (this->_connectCount < this->_ports.size())
            throw MessageException("Generate spice", "Not all port has been connected.");

        // Xins A B C D cir
        std::stringstream ss;

        // Instance name
        char deviceKeyword{ Instance::deviceKeywords[INT(this->_circuit->type())] }; 
        ss << deviceKeyword << this->_name;
        
        // Ports
        for (auto iter = this->_ports.begin(); iter != this->_ports.end(); ++iter)
            ss << ' ' << (*iter)->net()->name();
        
        // Circuit name
        ss << ' ' << this->_circuit->name();

        // Arguments list
        ss << ' ' << this->_circuit->argumentsList();

        return ss.str();
    }

    Port* Instance::port(const std::string& name) const
    {
        for (Port* port : this->_ports)
            if (port->name() == name)
                return port;
        return nullptr;
    }

}