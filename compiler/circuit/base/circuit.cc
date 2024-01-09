#include "circuit.hh"
#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include <allocator/allocator.hh>
#include <exception/msgexception.hh>

#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <fstream>

namespace xtaro::circuit
{
    Circuit::Circuit(std::string name) :
        _name{std::move(name)},
        _ports{},
        _circuits{},
        _instances{},
        _nets{},
        _isMetaCircuit{false},
        _metaSpice{}
    {
        
    }

    Circuit::~Circuit() noexcept
    {
        for (Circuit* circuit : this->_circuits)
            Allocator::free<Circuit>(circuit);
        
        for (Instance* instance : this->_instances)
            Allocator::free<Instance>(instance);

        for (Port* port : this->_ports)
            Allocator::free<Port>(port);
        
        for (auto& kv : this->_nets)
            Allocator::free<Net>(kv.second);
    }

    void Circuit::addPort(std::string name, PortType type)
    {
        Port* port = Allocator::alloc<Port>(std::move(name), type);
        this->_ports.push_back(port);
    }

    Instance* Circuit::addInstance(std::string instanceName, Circuit* circuit)
    {
        Instance* instance = Allocator::alloc<Instance>(std::move(instanceName), circuit);
        this->_instances.push_back(instance);
        return instance;
    }

    std::vector<const char*> Circuit::portsName() const
    {
        std::vector<const char*> names(this->_ports.size(), nullptr);
        std::size_t i = 0;
        
        for (Port* port : this->_ports)
            names[i++] = port->name().c_str();
        return names;
    }

    void Circuit::connectWith(Instance* instance, const std::vector<std::string>& netsName)
    {
        if (instance->ports().size() != netsName.size())
            throw MessageException("Connet Instance", "Port size != Net size");

        std::vector<Net*> nets = this->createNets(netsName);
        instance->connectNets(nets);
    }

    std::vector<Net*> Circuit::createNets(const std::vector<std::string>& netsName)
    {
        std::vector<Net*> nets(netsName.size(), nullptr);
        for (std::size_t i = 0; i < nets.size(); ++i)
        {
            auto iter = this->_nets.find( netsName[i] );
            if (iter != this->_nets.end())
                nets[i] = iter->second;
            else
            {   
                // Create a new Net
                Net* net = Allocator::alloc<Net>( netsName[i] );
                this->_nets.emplace(netsName[i], net);
                nets[i] = net;
            }
        }
        return nets;
    }

    std::vector<Port*> Circuit::copyPort() const
    {
        std::vector<Port*> ports(this->_ports.size(), nullptr);
        std::size_t index = 0;
        
        for (Port* port : this->_ports)
        {
            ports[index++] = Allocator::alloc<Port>(
                port->name(),
                port->type()
            );
        }
        return ports;
    }
    
    void Circuit::createNetlist()
    {
        this->createPorts();
        this->createSubckts();
        this->createInstances();
    }

    void Circuit::writeSpice(const std::string& filename)
    {
        // Create spice output file and visited set
        std::ofstream file(filename);
        std::set<Circuit*> visited;
        // Do write spice
        this->doWriteSpice(file, visited);
        file.close();
    }

    void Circuit::doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited)
    {
        if (this->_isMetaCircuit == false)
        {
            // .SUBCKT and2 A B Z vdd gnd  
            // Output sub circuits in 'this'
            for (Circuit* circuit : this->_circuits)
            {
                if (visited.find(circuit) != visited.end())
                    continue;
                circuit->doWriteSpice(file, visited);
            }

            // Write .SUBCKT line
            file << "\n.SUBCKT " << this->_name;
            for (Port* port : this->_ports)
                file << ' ' << port->name();
            file << '\n';

            // Write Instances
            for (Instance* instance : this->_instances)
            {
                std::string spiceCommand = instance->spiceCommand();
                file << spiceCommand << '\n';
            }

            // Write .END
            file << ".END " << this->_name << '\n'; 
        }
        else
        {
            file << '\n' << this->_metaSpice << '\n';
        }
    }
}