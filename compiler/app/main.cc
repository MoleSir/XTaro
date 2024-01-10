#include "xtaro.hh"
#include <iostream>

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        // TODO
        std::cout << "TODO: interpreter mode\n";
    }
    else 
    {
        xtaro::init(argv[1]);
        xtaro::createSRAM();
        xtaro::saveFiles();
    }

    return 0;
}