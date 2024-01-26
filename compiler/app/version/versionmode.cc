#include  "../xtaro/xtaro.hh"
#include <iostream>

namespace xtaro
{
    static const char* verionMessage {
        "Verion 0.11. Just SPICE, No GDSII."
    };

    void XTaro::runVersionMode(const std::vector<std::string>& arguments)
    {
        std::cout << verionMessage << std::endl;
    }

}