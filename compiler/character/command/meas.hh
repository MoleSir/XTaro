#pragma once

#include <string>
#include <fstream>

namespace xtaro::character
{

    class Measurement
    {
    public:
        Measurement(std::string name);

        virtual void writeCommand(std::ofstream& file) const = 0;
        virtual bool getMeasureResult(const std::string& context);

    public:
        const std::string& name() const noexcept
        { return this->_name; }

        double result() const noexcept
        { return this->_result; }

    protected:
        std::string _name;
        double _result;
    };
    
}