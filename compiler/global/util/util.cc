#include "util.hh"
#include <cstdarg>
#include <cstdlib>
#include <iostream>

namespace xtaro::util
{
    int execute(const std::string& command) noexcept
    {
        return system(command.c_str());
    }

    int execute(const char* command) noexcept
    {
       return system(command);
    }
}