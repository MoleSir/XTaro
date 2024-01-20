#include "xtaro/xtaro.hh"
#include <iostream>
#include <util/util.hh>

using namespace xtaro;

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        // TODO
        std::cout << "TODO: interpreter mode\n";
    }
    else 
    {
        xTaro->init(argv[1]);
        xTaro->createSRAM();
        xTaro->saveFiles();
    }

    return 0;
}