#include "port.hh"
#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{
    Port::Port(std::string name, PortType type):
        _name{std::move(name)},
        _type{type},
        _net{nullptr}
    {

    }

    
}