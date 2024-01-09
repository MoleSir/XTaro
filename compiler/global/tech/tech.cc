#include "tech.hh"
#include <config/config.hh>
#include <util/util.hh>
#include <exception/msgexception.hh>
#include <json/json.hh>

#include <iostream>

namespace xtaro
{
    Tech& tech{Tech::instance()};

    void Tech::load()
    {
        // Get tech file path and load it
        std::string techfile{ util::format("%s/%s/tech.json", 
                              config.techPath.c_str(),
                              config.techName.c_str()) };
        std::cout << techfile << std::endl;
        parse::Json json{ parse::Json::loadFromFile(techfile) };

        // Spice tech
        parse::Json spice{ json.get("spice") };
        if (spice.invalid())
            throw MessageException("Load tech", "No spice tech message.");

        std::string nmosName{"nmos"};
        if (spice.has(nmosName))
            this->spice.nmos.assign( std::move(spice[nmosName].asString()) );
        else
            throw MessageException("Load spice tech", "No 'nmos' message.");

        std::string pmosName{"pmos"};
        if (spice.has(pmosName))
            this->spice.pmos.assign( std::move(spice[pmosName].asString()) );
        else
            throw MessageException("Load spice tech", "No 'pmos' message.");
    }

    Tech::Tech()
    {
    }

    Tech& Tech::instance()
    {
        static Tech _tech{};
        return _tech;
    }  
}