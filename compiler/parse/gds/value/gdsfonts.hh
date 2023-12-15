#pragma once

#include "gdsvalue.hh"
#include <string>
#include <array>

namespace xtaro::parser
{

    class GDSFonts : public GDSValue
    {
    public:
        GDSFonts(const char* font1, const char* font2, const char* font3, const char* font4) : 
            _fonts{font1, font2, font3, font4} {}
        ~GDSFonts() noexcept {}

        const std::string& font1() const noexcept
        { return this->_fonts[0]; }

        const std::string& font2() const noexcept
        { return this->_fonts[1]; }

        const std::string& font3() const noexcept
        { return this->_fonts[2]; }

        const std::string& font4() const noexcept
        { return this->_fonts[3]; }

    private:
        std::array<std::string, 4> _fonts;
    };

}