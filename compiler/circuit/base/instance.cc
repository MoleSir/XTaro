#include "instance.hh"
#include "net.hh"
#include "circuit.hh"
#include "port.hh"

#include "allocator/allocator.hh"

#include <string>
#include <map>
#include <list>
#include <string>

namespace xtaro::circuit
{
    Instance::Instance(std::string name, Circuit* circuit) :
        _name{std::move(name)},
        _circuit{circuit},
        _ports{} {}

    Instance::~Instance() noexcept
    {
        for (const auto& keyValue : this->_ports)
            Allocator::free(keyValue.second);
    }
}