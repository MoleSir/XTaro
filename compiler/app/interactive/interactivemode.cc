#include "../xtaro/xtaro.hh"
#include "interactivextaro.hh"

#include <debug/debug.hh>
#include <util/file.hh>

#include <string>
#include <iostream>

namespace xtaro 
{

    void XTaro::runInteractiveMode(const std::vector<std::string>& arguments)
    {
        debug->init(util::absolutePath("./") + "xtaro.log");
        InteractiveXTaro* xtaro {InteractiveXTaro::instance()};

        while (true)
        {
            xtaro->getCommandLine();
            
            try
            {
                xtaro->interprete();
            }
            catch (const InteractiveExit& err)
            {
                debug->info("Exit from interpreter mode.");
                break;
            }
        }
    }

}