#include "./measurement.hh"
#include "log/log.hh"
#include <regex>

namespace xtaro::simulate
{
    std::regex Measurement::_nameResultRegex{"delay\\s*=\\s*-?\\d+.?\\d*[e]?[-+]?[0-9]*"};
    std::regex Measurement::_resultRegex{"-?\\d+.?\\d*[e]?[-+]?[0-9]*"};

    Measurement::Measurement(std::string name) : 
        _name{std::move(name)}, _result{0.0} 
    {
    }

    bool Measurement::getMeasureResult(const std::string& context)
    {
        // Find 'name = result'
        std::smatch nameResultMatch;
        bool success = 
            std::regex_search(context, nameResultMatch, Measurement::_nameResultRegex);
        if (success == false)
        {
            logger->warning("Geg .meas result failed, no 'name = result' found!");
            return false;
        }
        
        // Find 'result'
        std::string nameValue{nameResultMatch[0].str()};
        std::smatch resultMatch;
        // Note: If the 'context' pass the first regex_search, it must can pass this one
        std::regex_search(nameValue, resultMatch, Measurement::_resultRegex);

        // Get value
        this->_result = std::stod(resultMatch[0].str());
        return true;
    }
    
}