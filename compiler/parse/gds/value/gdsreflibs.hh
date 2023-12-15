#pragma once

#include "gdsvalue.hh"
#include <string>
#include <array>

namespace xtaro::parser
{

    class GDSRefenceLibs : public GDSValue
    {
    public:
        GDSRefenceLibs(const char* lib1, const char* lib2) : 
            GDSValue(), _libs{lib1, lib2} {}
        ~GDSRefenceLibs() noexcept {}

        const std::string& lib1() const noexcept
        { return this->_libs[0]; }

        const std::string& lib2() const noexcept
        { return this->_libs[1]; }

    private:
        std::array<std::string, 2> _libs;
    };

}