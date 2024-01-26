#include "../xtaro/xtaro.hh"
#include "interpreterxtaro.hh"

#include <debug/debug.hh>
#include <util/file.hh>

#include <string>
#include <iostream>

namespace xtaro 
{

    void XTaro::runInterpreterMode(const std::vector<std::string>& arguments)
    {
        debug->init(util::absolutePath("./") + "xtaro.log");
        InterpreterXTaro* xtaro {InterpreterXTaro::instance()};

        while (true)
        {
            xtaro->getCommandLine();
            
            try
            {
                xtaro->interprete();
            }
            catch (const InterpreterExit& err)
            {
                debug->info("Exit from interpreter mode.");
                break;
            }
        }
    }

}