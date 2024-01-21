#include "sys.hh"
#include <cstdlib>

namespace xtaro::util
{
    int execute(const std::string& command) noexcept
    {
        return std::system(command.c_str());
    }

    int execute(const char* command) noexcept
    {
       return std::system(command);
    }
}