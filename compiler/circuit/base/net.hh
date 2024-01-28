#pragma once

#include <string>
#include <string_view>

namespace xtaro::circuit
{
    
    class Port;

    class Net
    {
    public:
        Net(const std::string_view& name);
        const std::string_view& name() const noexcept
        { return this->_name; }
                
    private:
        std::string_view _name;
    };

}