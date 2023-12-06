#include "circuit.hh"
#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include "allocator/allocator.hh"

#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>

namespace xtaro::circuit
{
    Circuit::Circuit(std::string name) :
        _name{std::move(name)},
        _ports{},
        _circuits{},
        _instances{} {}

    Circuit::~Circuit() noexcept
    {
        for (const auto& kv : this->_ports)
            Allocator::free(kv.second);

        for (Circuit* circuit : this->_circuits)
            Allocator::free(circuit);

        for (Instance* instance : this->_instances)
            Allocator::free(instance);
    }

    void Circuit::addPort(std::string name, PortType type)
    {
        Port* port = Allocator::alloc<Port>(std::move(name), type);
        port->setCircuit(this);
        this->_ports[port->name()] = port;
    }

    std::vector<const char*> Circuit::portsName() const
    {
        std::vector<const char*> names(this->_ports.size(), nullptr);
        std::size_t i = 0;
        
        for (const auto& kv : this->_ports)
        {
            names[i] = kv.second->name().c_str();  
            i += 1;
        }

        return names;
    }
}