#include <xtaro/xtaro.hh>
#include "scriptxtaro.hh"

#include <iostream>

namespace xtaro 
{

    void XTaro::runScriptMode(const std::vector<std::string>& arguments)
    {
        ScriptXTaro* xtaro {ScriptXTaro::instance()};
        xtaro->init(arguments[0]);
        xtaro->createSRAM();
        xtaro->saveFiles();
    }

}