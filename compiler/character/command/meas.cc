#include "meas.hh"

#include <log/logger.hh>
#include <util/util.hh>
#include <regex>
#include <iostream>

namespace xtaro::character
{

    Measurement::Measurement(std::string name) : 
        _name{std::move(name)}, _result{0.0} 
    {
    }

    bool Measurement::getMeasureResult(const std::string& context)
    {
        std::regex nameResultRegex{
            util::format("%s\\s*=\\s*-?\\d+.?\\d*[e]?[-+]?[0-9]", this->_name.c_str())
        };

        // Find 'name = result'
        std::smatch nameResultMatch;
        bool success = 
            std::regex_search(context, nameResultMatch, nameResultRegex);
        if (success == false)
        {
            logger->warning(util::format(
                "Get '%s'.meas result failed!", this->_name.c_str()).c_str());
            return false;
        }
        
        // Find 'result'
        std::string nameValue{nameResultMatch[0].str()};
        std::size_t pos {nameValue.find('=')};
        while (nameValue[++pos] != ' ') {}

        // Get value
        this->_result = std::stod(&(nameValue[pos]));
        return true;
    }
    
}