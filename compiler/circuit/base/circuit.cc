#include "circuit.hh"
#include "instance.hh"
#include "net.hh"
#include "port.hh"

#include <allocator/allocator.hh>
#include <exception/msgexception.hh>
#include <log/logger.hh>
#include <util/util.hh>

#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <fstream>

namespace xtaro::circuit
{

    Circuit::Circuit(String name, DeviceType type, const std::string& spicefile) :
        _name{std::move(name)},
        _type{type},
        _ports{},
        _circuits{},
        _instances{},
        _nets{},
        _isMetaCircuit{false},
        _metaSpice{}
    {
        if (spicefile.empty())
            return;

        std::ifstream file{spicefile};
        this->_metaSpice.assign(util::readFile(file));
        file.close();

        this->_isMetaCircuit = true;
    }

    Circuit::~Circuit() noexcept
    {        
        for (Instance* instance : this->_instances)
            Allocator::free<Instance>(instance);

        for (Port* port : this->_ports)
            Allocator::free<Port>(port);
        
        for (auto& kv : this->_nets)
            Allocator::free<Net>(kv.second);
    }

    void Circuit::addPort(String name, PortType type)
    {
        Port* port { Allocator::alloc<Port>(name, type) };
        this->_ports.emplace_back(port);
    }

    void Circuit::addPorts(const std::vector<String>& portsName, PortType portType)
    {
        for (const String& portName : portsName)
            this->addPort(portName, portType);
    }

    Instance* Circuit::addInstance(String instanceName, Circuit* circuit)
    {
        Instance* instance = Allocator::alloc<Instance>(instanceName, circuit);
        this->_instances.emplace_back(instance);
        return instance;
    }

    void Circuit::connectWith(Instance* instance, const std::vector<String>& netsName)
    {
        if (instance->ports().size() != netsName.size())
        {
            std::string errorMsg {
                util::format("In circuit '%s', when connect with instance '%s', port size '%d' != Net size '%d'", 
                             this->_name.cstr(),
                             instance->name().cstr(),
                             instance->ports().size(), 
                             netsName.size())
            };

            logger->error(errorMsg);
            throw MessageException("Connect Instance", errorMsg);
        }

        std::vector<Net*> nets = this->createNets(netsName);
        instance->connectNets(nets);
    }

    Instance* Circuit::addInstance(String instanceName, Circuit* circuit, const std::vector<String>& netsName)
    {
        Instance* instance {this->addInstance(instanceName, circuit)};
        this->connectWith(instance, netsName);
        return instance;
    }

    std::vector<std::string> Circuit::portsName() const
    {
        std::vector<std::string> names{};     
        for (Port* port : this->_ports)
            names.emplace_back(std::string{port->name().cstr()});
        return names;
    }

    std::vector<Net*> Circuit::createNets(const std::vector<String>& netsName)
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
        std::vector<Port*> ports{};        
        for (Port* port : this->_ports)
        {
            ports.emplace_back(Allocator::alloc<Port>(
                // TAG: Do not copy port
                port->name(),
                port->type()
            ));
        }
        return ports;
    }
    
    void Circuit::createNetlist()
    {
        this->createPorts();
        this->createCircuits();
        this->createInstances();
    }

    void Circuit::writeSpice(const std::string& filename)
    {
        // Create spice output file and visited set
        std::ofstream file{filename};
        std::set<Circuit*> visited{};
        // Do write spice
        this->doWriteSpice(file, visited);
        file.close();
    }

    void Circuit::doWriteSpice(std::ofstream& file, std::set<Circuit*>& visited)
    {
        visited.emplace(this);
        
        if (this->_isMetaCircuit == false)
        {
            // .SUBCKT and2 A B Z vdd gnd  
            // Output sub circuits in 'this'
            for (Circuit* circuit : this->_circuits)
            {
                if (circuit->type() != DeviceType::SUBCKT ||
                    visited.find(circuit) != visited.end())
                    continue;
                circuit->doWriteSpice(file, visited);
            }

            // Write .SUBCKT line
            file << "\n.SUBCKT " << this->_name.cstr() << '\n';
            // Write ports, each line has 15 ports
            int portsSize{0};
            for (Port* port : this->_ports)
            {
                if (portsSize == 0) file << '+';
                file << ' ' << port->name();
                if (++portsSize == 15)
                {
                    file << '\n';
                    portsSize = 0;
                }
            }
            file << '\n';

            // Write Instances
            for (Instance* instance : this->_instances)
            {
                std::string spiceCommand{instance->spiceCommand()};
                file << spiceCommand << '\n';
            }

            // Write .END
            file << ".ENDS " << this->_name.cstr() << '\n';
        }
        else
        {
            file << '\n' << this->_metaSpice << '\n';
        }
    }
}