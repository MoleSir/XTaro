#include <xtaro/xtaro.hh>
#include "scriptxtaro.hh"

#include <iostream>

namespace xtaro 
{

    void XTaro::runScriptMode(const std::vector<std::string>& arguments)
    {
        try
        {
            ScriptXtaro* xtaro {ScriptXtaro::instance()};
            xtaro->init(arguments[0]);
            xtaro->createSRAM();
            xtaro->saveFiles();
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
            std::exit(12);
        }
    }

}