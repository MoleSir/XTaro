#include "interactivextaro.hh"

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

    InteractiveXTaro::InteractiveXTaro() : 
        _methodMap 
        {
            {std::string{"load_option"}, std::bind(&InteractiveXTaro::loadOption, this)},
            {std::string{"cat_option"}, std::bind(&InteractiveXTaro::catOption, this)},

            {std::string{"compile"}, std::bind(&InteractiveXTaro::compile, this)},

            {std::string{"save"}, std::bind(&InteractiveXTaro::save, this)},
            {std::string{"saveSpice"}, std::bind(&InteractiveXTaro::saveSpice, this)},
            {std::string{"saveVerilog"}, std::bind(&InteractiveXTaro::saveVerilog, this)},
            
            {std::string{"function_test"}, std::bind(&InteractiveXTaro::functionTest, this)},
            
            {std::string{"exit"}, std::bind(&InteractiveXTaro::exit, this)},
            {std::string{"clear"}, std::bind(&InteractiveXTaro::clear, this)},

            {std::string{"set_debug_level"}, std::bind(&InteractiveXTaro::setDebugLevel, this)},
        }
    {
    }

    InteractiveXTaro* InteractiveXTaro::instance()
    {
        static InteractiveXTaro _xtaro;
        return &_xtaro;
    }

    void InteractiveXTaro::interprete()
    {
        if (this->parse())
            this->execute();
    }

    void InteractiveXTaro::getCommandLine()
    {
        std::cout << "XTaro> ";

        this->_commandLine.clear();
        auto history {this->_historyCommands.begin()};

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
            {
                // Page Up and Page Down start with '\33', follow by '[A' or '[B'
                util::getCharNoEcho();
                ch = util::getCharNoEcho();
                
                switch (ch)
                {
                // Page Up
                case 'A':
                {
                    auto current = history;
                    if (++history == this->_historyCommands.end())
                        history = current;
                    break;
                }
                // Page Up
                case 'B':
                {
                    if (history != this->_historyCommands.begin())
                        --history;
                    break;
                }
                // Page Left
                case 'D':
                {
                    break;
                }
                case 'C':
                {
                    break;
                }
                default: break;
                }

                this->clearCurrentInput();
                this->_commandLine.assign(*history);
                std::cout << this->_commandLine;

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

    bool InteractiveXTaro::checkOption() const
    {
        if (this->_loadOptionSucc == false)
        {
            debug->error("XTaro has not been configed, please 'load_option xx.json' firstly~");
            return false;
        }
        return true;
    }

    bool InteractiveXTaro::checkCompile() const
    {
        if (this->_compileSucc == false)
        {
            debug->error("SRAM has not been compiled, please 'compile' firstly~");
            return false;
        }
        return true;
    }

    void InteractiveXTaro::clearCurrentInput()
    {
        for (std::size_t i = 0; i < this->_commandLine.size(); ++i)
        {
            std::putchar('\b');
            std::putchar(' ');
            std::putchar('\b');
        }
        this->_commandLine.clear();
    }

    void InteractiveXTaro::execute()
    {
        if (this->_method != nullptr)
            this->_method();
    }

    bool InteractiveXTaro::parse()
    {
        if (this->_commandLine.empty()) return false;

        // Save command line
        auto front {++this->_historyCommands.begin()};
        if (this->_commandLine != *front)
            this->_historyCommands.emplace(front, std::move(this->_commandLine));

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
    InteractiveXTaro::splitCommandLine()
    {
        std::vector<std::string> tokens {};
        std::size_t begin {0};
        std::size_t end {0};

        const std::string& commandLine {*(++this->_historyCommands.begin())};

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
