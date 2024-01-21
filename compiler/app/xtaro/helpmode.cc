#include "xtaro.hh"
#include <iostream>

namespace xtaro
{
    // TODO Exit status
    const char* XTaro::helpMessage {
        "Usage: xtaro [options] file\n"
        "Options:\n"
        "   -h, --help         Display this information.\n"
        "   -v, --verion       Display compiler version information.\n"
        "   -l, --load <file>  Load options from <file> and start compiling.\n"
        "   -i, --interactive  Interactive mode.\n"
        "\n"
        "Exit status:\n"
        "   0  if Ok\n"
    };

    void XTaro::runHelpMode(const std::vector<std::string>& arguments)
    {
        std::cout << XTaro::helpMessage << std::endl;
    }

}