#include "xtaro.hh"
#include <iostream>

namespace xtaro
{
    const char* XTaro::verionMessage {
        "Verion 0.11. Just SPICE, No GDSII."
    };

    void XTaro::runVersionMode(const std::vector<std::string>& arguments)
    {
        std::cout << XTaro::verionMessage << std::endl;
    }

}