#pragma once

#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{
    class Port;

    class Net
    {
    public:
        Net(std::string name);
        const std::string& name() const noexcept
        { return this->_name; }
                
    private:
        std::string _name;
    };

}