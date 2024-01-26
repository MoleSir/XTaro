#include "interpreterxtaro.hh"
#include <debug/exception.hh>
#include <util/format.hh>
#include <debug/debug.hh>

#include <iostream>

namespace xtaro 
{
    std::map<std::string, InterpreterXTaro::Command> 
    InterpreterXTaro::commandMap 
    {
        {std::string{"load_option"}, Command::LOAD_OPTION},
        {std::string{"compile"}, Command::COMPILE},
        {std::string{"function_test"}, Command::FUNCTION_TEST},
        {std::string{"exit"}, Command::EXIT},
    };

    InterpreterXTaro* InterpreterXTaro::instance()
    {
        static InterpreterXTaro _xtaro;
        return &_xtaro;
    }

    void InterpreterXTaro::interprete(std::string commandLine)
    {
        if (this->parse(std::move(commandLine)))
            this->execute();
    }

    void InterpreterXTaro::loadOption()
    {
        std::cout << "loadOption" << std::endl;
    }

    void InterpreterXTaro::compile()
    {
        std::cout << "compile" << std::endl;
    }

    void InterpreterXTaro::functionTest()
    {
        std::cout << "functionTest" << std::endl;
    }

    void InterpreterXTaro::exit()
    {
        throw InterpreterExit{};
    }

    void InterpreterXTaro::execute()
    {
        switch (this->_command)
        {
        case Command::LOAD_OPTION:   return this->loadOption();
        case Command::COMPILE:       return this->compile();
        case Command::FUNCTION_TEST: return this->functionTest();
        case Command::EXIT:          return this->exit();
        default: return;
        }
    }

    bool InterpreterXTaro::parse(std::string commandLine)
    {
        this->_command = Command::NONE;
        this->_arguments.clear();

        // Split command line into small tokens
        std::vector<std::string> tokens {this->splitCommandLine(commandLine)};

        // first token is command
        auto res {InterpreterXTaro::commandMap.find(tokens[0])};
        if (res == commandMap.end())
        {
            std::cout << util::format("Unkonw command: '%s'", tokens[0].c_str()) << std::endl;
            return false;
        }

        this->_command = res->second;

        // other tokens are arguments   
        for (std::size_t i = 1; i < tokens.size(); ++i)
            this->_arguments.emplace_back(std::move(tokens[i]));        

        // Save command line
        this->_historyCommands.emplace_back(std::move(commandLine));
        return true;
    }

    std::vector<std::string> 
    InterpreterXTaro::splitCommandLine(const std::string& commandLine)
    {
        std::vector<std::string> tokens {};
        std::size_t begin {0};
        std::size_t end {0};

        while (begin < commandLine.size())
        {
            if (commandLine[begin] == ' ')
            {
                begin += 1;
                break;
            }
            // 'commandLine[begin]' is the begin fo one token
            // Find next ' ' or the end of 'commandLine'
            end = begin + 1;
            while (true)
            {
                if (end >= commandLine.size() || commandLine[end] == ' ')
                    break;
                end += 1;
            }
            // 'commandLine[begin : end)' is a token
            const char* tokenStr {&(commandLine[begin])};
            std::size_t tokenSize {end - begin};
            tokens.emplace_back(std::string(tokenStr, tokenSize));
            // Update begin
            begin = end + 1;
        }

        return tokens;
    }

}
