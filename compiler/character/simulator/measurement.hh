#pragma once

#include <string>
#include <fstream>
#include <regex>

namespace xtaro::simulate
{

    class Measurement
    {
        static std::regex _nameResultRegex;
        static std::regex _resultRegex;

    public:
        Measurement(std::string name);

        virtual void writeCommand(std::ofstream& file) const = 0;

        bool getMeasureResult(const std::string& context);

        const std::string& name() const noexcept
        { return this->_name; }

        double result() const noexcept
        { return this->_result; }

    protected:
        std::string _name;
        double _result;
    };
    
}