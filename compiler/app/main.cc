#include "xtaro.hh"
#include <iostream>

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