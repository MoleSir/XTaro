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

    std::string readFile(std::ifstream& file)
    {
        std::size_t size {filesize(file)};
        std::string content {};
        content.resize(size + 1);
        file.read(content.data(), size);
        content.reserve(size);
        return content;
    }

    int power(int base, unsigned int exponent)
    {
        int value = 1;
        for (unsigned int i = 0; i < exponent; ++i)
            value *= base;
        return value;
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