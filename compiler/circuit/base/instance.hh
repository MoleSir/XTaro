#pragma once

#include "circuitenum.hh"
#include <stringpool/string.hh>

#include <string>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <array>

namespace xtaro::circuit
{
    class Circuit;
    class Port;
    class Net;

    class Instance
    {
        static const std::array<char, DEVICE_SIZE> deviceKeywords;

    public:
        Instance(String name, Circuit* circuit);
        ~Instance() noexcept;

    public:
        //! \brief Connect 'net' to the 'port' whose name is 'portName'
        void connectNet(const String& portName, Net* net);
        void connectNets(const std::vector<Net*>& nets);

        //! \brief Generate the spice command of this instance
        std::string spiceCommand() const;

    public:
        Port* port(const String& name) const;

        const String& name() const noexcept
        { return this->_name; }

        Circuit* circuit() const noexcept
        { return this->_circuit; }

        const std::vector<Port*> ports() const noexcept
        { return this->_ports; }

    private:
        String _name;
        Circuit* _circuit;
        std::vector<Port*> _ports;
        std::size_t _connectCount;
    };

}