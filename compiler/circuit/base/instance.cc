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

    Instance::Instance(String name, Circuit* circuit) :
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

    void Instance::connectNet(const String& portName, Net* net)
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
            throw MessageException(
                "Generate spice", 
                util::format("The instance '%s', belongs circuit '%s'. Not all port has been connected. Connected: '%d'.", 
                    this->_name.cstr(), this->_circuit->name().cstr(), this->_connectCount)
            );

        // Xins A B C D cir
        std::stringstream ss;

        // Instance name
        char deviceKeyword{ Instance::deviceKeywords[INT(this->_circuit->type())] }; 
        ss << deviceKeyword << this->_name.cstr();
   
        // Ports
        if (this->_circuit->type() == DeviceType::SUBCKT)
        {
            ss << '\n';
            int netsSize{0};
            for (Port* port : this->_ports)
            {
                if (netsSize == 0) ss << '+';
                ss << ' ' << port->net()->name();
                if (++netsSize == 15)
                {
                    ss << '\n';
                    netsSize = 0;
                }
            }
            ss << '\n' << '+';
        }
        else
        {
            for (auto iter = this->_ports.begin(); iter != this->_ports.end(); ++iter)
                ss << ' ' << (*iter)->net()->name();
        }
        
        // Circuit name
        ss << ' ' << this->_circuit->name().cstr();

        // Arguments list
        ss << ' ' << this->_circuit->argumentsList();

        return ss.str();
    }

    Port* Instance::port(const String& name) const
    {
        for (Port* port : this->_ports)
            if (port->name() == name)
                return port;
        return nullptr;
    }

}