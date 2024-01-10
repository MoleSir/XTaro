#include "util.hh"
#include <cstdarg>
#include <cstdlib>
#include <iostream>

namespace xtaro::util
{
    std::size_t filesize(std::ifstream& file)
    {
        file.seekg(0, std::ios::end);
        std::size_t size{ static_cast<std::size_t>(file.tellg()) };
        file.seekg(0, std::ios::beg);
        return size;
    }

    int execute(const std::string& command) noexcept
    {
        return system(command.c_str());
    }

    int execute(const char* command) noexcept
    {
       return system(command);
    }
}