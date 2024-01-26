#include "interpreterxtaro.hh"

#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

#include <config/option.hh>
#include <config/tech.hh>
#include <util/format.hh>
#include <util/sys.hh>
#include <debug/debug.hh>
#include <debug/exception.hh>

#include <iostream>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace xtaro 
{

    InterpreterXTaro::InterpreterXTaro() : 
        _methodMap 
        {
            {std::string{"load_option"}, std::bind(&InterpreterXTaro::loadOption, this)},
            {std::string{"cat_option"}, std::bind(&InterpreterXTaro::catOption, this)},

            {std::string{"compile"}, std::bind(&InterpreterXTaro::compile, this)},

            {std::string{"save"}, std::bind(&InterpreterXTaro::save, this)},
            {std::string{"saveSpice"}, std::bind(&InterpreterXTaro::saveSpice, this)},
            {std::string{"saveVerilog"}, std::bind(&InterpreterXTaro::saveVerilog, this)},
            
            {std::string{"function_test"}, std::bind(&InterpreterXTaro::functionTest, this)},
            
            {std::string{"exit"}, std::bind(&InterpreterXTaro::exit, this)},
            {std::string{"clear"}, std::bind(&InterpreterXTaro::clear, this)},
        }
    {
    }

    InterpreterXTaro* InterpreterXTaro::instance()
    {
        static InterpreterXTaro _xtaro;
        return &_xtaro;
    }

    void InterpreterXTaro::interprete()
    {
        if (this->parse())
            this->execute();
    }

    void InterpreterXTaro::getCommandLine()
    {
        std::cout << "XTaro> ";

        this->_commandLine.clear();
        auto history {this->_historyCommands.rbegin()};

        bool finish{false};
        while (!finish)
        {
            char ch {util::getCharNoEcho()};

            switch (ch)
            {
            case '\n':
            {
                std::cout << std::endl;
                finish = true;
                break;
            }
            case 127:
            {
                if (this->_commandLine.empty()) break;
                this->_commandLine.pop_back();
                std::putchar('\b');
                std::putchar(' ');
                std::putchar('\b');
                break;
            }
            case '\33':
            case '\34':
            {
                this->clearCurrentInput();
                this->_commandLine.assign(*history);
                std::cout << this->_commandLine;
                
                if (ch == '\33')
                {
                    auto next {history + 1};
                    if (next != this->_historyCommands.rend())
                        history = next;
                }
                else 
                {
                if (history != this->_historyCommands.rbegin())
                    --history; 
                }

                // Jump two left charachters
                util::getCharNoEcho(), util::getCharNoEcho();
                continue;
            }
            default:
            {
                this->_commandLine.push_back(ch);
                std::putchar(ch);
                break;
            }
            }
        }
    }

    bool InterpreterXTaro::checkOption() const
    {
        if (this->_loadOptionSucc == false)
        {
            debug->error("XTaro has not been configed, please 'load_option xx.json' firstly~");
            return false;
        }
        return true;
    }

    bool InterpreterXTaro::checkCompile() const
    {
        if (this->_compileSucc == false)
        {
            debug->error("SRAM has not been compiled, please 'compile' firstly~");
            return false;
        }
        return true;
    }

    void InterpreterXTaro::clearCurrentInput()
    {
        for (std::size_t i = 0; i < this->_commandLine.size(); ++i)
        {
            std::putchar('\b');
            std::putchar(' ');
            std::putchar('\b');
        }
        this->_commandLine.clear();
    }

    void InterpreterXTaro::execute()
    {
        if (this->_method != nullptr)
            this->_method();
    }

    bool InterpreterXTaro::parse()
    {
        if (this->_commandLine.empty()) return false;

        // Save command line
        this->_historyCommands.emplace_back(std::move(this->_commandLine));

        // Split command line into small tokens
        std::vector<std::string> tokens {this->splitCommandLine()};

        // First token is command
        auto res {this->_methodMap.find(tokens[0])};
        if (res == this->_methodMap.end())
        {
            debug->error("Unkonw command: '%s'~", tokens[0].c_str());
            return false;
        }

        this->_method = nullptr;
        this->_arguments.clear();

        this->_method = res->second;

        // other tokens are arguments   
        for (std::size_t i = 1; i < tokens.size(); ++i)
            this->_arguments.emplace_back(std::move(tokens[i]));        

        return true;
    }

    std::vector<std::string> 
    InterpreterXTaro::splitCommandLine()
    {
        std::vector<std::string> tokens {};
        std::size_t begin {0};
        std::size_t end {0};

        const std::string& commandLine {this->_historyCommands.back()};

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
