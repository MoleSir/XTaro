#include "../xtaro/xtaro.hh"
#include "scriptxtaro.hh"

#include <debug/debug.hh>
#include <util/file.hh>

#include <iostream>

namespace xtaro 
{

    void XTaro::runScriptMode(const std::vector<std::string>& arguments)
    {
        debug->init(util::absolutePath("./") + "xtaro.log");
        ScriptXTaro* xtaro {ScriptXTaro::instance()};
        xtaro->init(arguments[0]);
        xtaro->createSRAM();
        xtaro->saveFiles();
    }

}