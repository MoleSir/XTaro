#pragma once

#include <stringpool/string.hh>

#include <string>
#include <map>
#include <list>

namespace xtaro::circuit
{
    
    class Port;

    class Net
    {
    public:
        Net(String name);
        const String& name() const noexcept
        { return this->_name; }
                
    private:
        String _name;
    };

}