#include "xtaro.hh"

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace xtaro
{
    XTaro::Mode XTaro::mode {XTaro::Mode::NONE};
    
    std::map<std::string, XTaro::Mode> XTaro::argvMap {
        {"-h", XTaro::Mode::HELP},
        {"--help", XTaro::Mode::HELP},
        {"-v", XTaro::Mode::VERION},
        {"--version", XTaro::Mode::VERION},
        {"-i", XTaro::Mode::INTERACTIVE},
        {"--interactive", XTaro::Mode::INTERACTIVE},
        {"-l", XTaro::Mode::SCRIPT},
        {"--load", XTaro::Mode::SCRIPT},
    };

    const char* XTaro::logo {
        " __  __ _____\n"
        " \\ \\/ /|_   _|__ _  _ __  ___\n" 
        "  \\  /   | | / _` || '__|/ _ \\\n" 
        "  /  \\   | || (_| || |  | (_) |\n"
        " /_/\\_\\  |_| \\__,_||_|   \\___/\n"
    };

    std::vector<std::string> 
    XTaro::parse(int argc, const char* argv[])
    {
        if (argc == 1)
        {
            XTaro::mode = XTaro::Mode::INTERACTIVE;
            return {};
        }

        std::vector<std::string> arguments{};
        for (int i = 1; i < argc; ++i)
            arguments.emplace_back(argv[i]);

        // First argument indicates mode
        auto iter {XTaro::argvMap.find(arguments[0])};
        if (iter == XTaro::argvMap.end())
        {
            if (arguments[0].front() == '-')
                std::cout << '"' << arguments[0] << '"' 
                          << " is not a supported option." << std::endl;
                          
            else 
                std::cout << "First argument should be an option." << std::endl;
            std::cout << "You can use '-h' or '--help' to the see how to run this tool." << std::endl;
            std::exit(10);
        }

        XTaro::mode = iter->second;
        if (XTaro::mode == XTaro::Mode::SCRIPT)
            return {arguments[1]};
        return {};
    }

    void XTaro::showLogo()
    {
        std::cout << XTaro::logo << std::endl;   
    }

    void XTaro::run(const std::vector<std::string>& arguments)
    {
        XTaro::showLogo();
        
        switch (XTaro::mode)
        {
        case XTaro::Mode::HELP: 
            return XTaro::runHelpMode(arguments);
        case XTaro::Mode::VERION:
            return XTaro::runVersionMode(arguments);
        case XTaro::Mode::INTERACTIVE:
            return XTaro::runInteractiveMode(arguments);
        case XTaro::Mode::SCRIPT:
            return XTaro::runScriptMode(arguments);
        case XTaro::Mode::NONE:
            return;
        }
    }

}
