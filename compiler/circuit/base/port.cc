#include "port.hh"
#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{

    Port::Port(const std::string_view& name, PortType type):
        _name{name},
        _type{type},
        _net{nullptr}
    {

    }

}