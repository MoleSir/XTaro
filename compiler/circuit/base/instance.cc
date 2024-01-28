#include "instance.hh"
#include "net.hh"
#include "circuit.hh"
#include "port.hh"

#include <allocator/allocator.hh>
#include <util/format.hh>
#include <debug/debug.hh>

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

    Instance::Instance(const std::string_view& name, Circuit* circuit) :
        _name{name},
        _circuit{circuit},
        _ports{circuit->copyPort()},
        _connectCount{0} 
    { 
    }

    Instance::~Instance() noexcept
    {
        for (Port* port : this->_ports)
            Allocator::free<Port>(port);
    }

    void Instance::connectNet(const std::string_view& portName, Net* net)
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
        {
            std::string errorMsg {
                util::format("The instance '%s', belongs circuit '%s'. Not all port has been connected. Connected: '%d'.", 
                             this->_name.data(), 
                             this->_circuit->name().data(), 
                             this->_connectCount)
            };

            debug->errorWithException("Generate Instance spice", errorMsg);
        }

        // Xins A B C D cir
        std::stringstream ss;

        // Instance name
        char deviceKeyword{ Instance::deviceKeywords[INT(this->_circuit->type())] }; 
        ss << deviceKeyword << this->_name.data();
   
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
        ss << ' ' << this->_circuit->name().data();

        // Arguments list
        ss << ' ' << this->_circuit->argumentsList();

        return ss.str();
    }

    Port* Instance::port(const std::string_view& name) const
    {
        for (Port* port : this->_ports)
            if (port->name() == name)
                return port;
        return nullptr;
    }

}