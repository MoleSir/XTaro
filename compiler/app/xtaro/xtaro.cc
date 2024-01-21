#include "xtaro.hh"

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace xtaro
{
    XTaro* xTaro {XTaro::instance()};

    std::map<std::string, RunMode> XTaro::argvMap {
        {"-h", RunMode::HELP},
        {"--help", RunMode::HELP},
        {"-v", RunMode::VERION},
        {"--version", RunMode::VERION},
        {"-i", RunMode::INTERPRETER},
        {"--interactive", RunMode::INTERPRETER},
        {"-l", RunMode::SCRIPT},
        {"--load", RunMode::SCRIPT},
    };

    std::vector<std::string> 
    XTaro::parseArgv(int argc, const char* argv[])
    {
        if (argc == 1)
        {
            this->_runMode = RunMode::INTERPRETER;
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

        this->_runMode = iter->second;
        if (this->_runMode == RunMode::SCRIPT)
            return {arguments[1]};
        return {};
    }

    void XTaro::run(const std::vector<std::string>& arguments)
    {
        switch (this->_runMode)
        {
        case RunMode::HELP: 
            return this->runHelpMode(arguments);
        case RunMode::VERION:
            return this->runVersionMode(arguments);
        case RunMode::INTERPRETER:
            return this->runInterpreterMode(arguments);
        case RunMode::SCRIPT:
            return this->runScriptMode(arguments);
        }
    }

    XTaro* XTaro::instance()
    {
        static XTaro _xTaro{};
        return &_xTaro;
    }
}
