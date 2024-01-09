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
            port->setNet(net);
    }

    void Instance::connectNets(const std::vector<Net*>& nets)
    {
        for (std::size_t i = 0; i < this->_ports.size(); ++i)
            this->_ports[i]->setNet(nets[i]);
    }

    std::string Instance::spiceCommand() const
    {
        if (this->_connectCount < this->_ports.size())
            throw MessageException("Generate spice", "Not all port has been connected.");

        // Xins A B C D cir
        std::stringstream ss;
        ss << 'X' << this->_name;
        for (auto iter = this->_ports.begin(); iter != this->_ports.end(); ++iter)
            ss << ' ' << (*iter)->name();
        ss << ' ' << this->_circuit->name();

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