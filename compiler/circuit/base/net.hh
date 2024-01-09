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
                
    private:
        std::string _name;
    };

}