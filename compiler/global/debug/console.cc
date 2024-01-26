#include "console.hh"
#include <util/format.hh>
#include <iostream>

namespace xtaro
{

    void Console::debug(const std::string& message) const
    {
        this->print("\e[34m DEBUG \e[0m", message);
    }

    void Console::info(const std::string& message) const
    {
        this->print("\e[32m INFO  \e[0m", message);
    }

    void Console::warning(const std::string& message) const
    {
        this->print("\e[33mWARNING\e[0m", message);
    }

    void Console::error(const std::string& message) const
    {
        this->print("\e[31m ERROR \e[0m", message);
    }

    // void Console::fatal(const std::string& message) const
    // {
    //     this->print("\e[31mFATAL\e[0m", message);
    // }
    
    void Console::print(const char* level, const std::string& message) const
    {
        this->printLevel(level);
        this->printMessage(message);
    }

    void Console::printMessage(const std::string& message) const
    {
        std::cout << message << std::endl;
    }

    void Console::printLevel(const char* level) const
    {
        std::cout << '[' << level << "]: ";
    }
}