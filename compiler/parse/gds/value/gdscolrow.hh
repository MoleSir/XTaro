#pragma once

#include "gdsvalue.hh"
#include <cstdint>

namespace xtaro::parser
{

    class GDSColumnRow : public GDSValue
    {
    public:
        GDSColumnRow(std::int16_t column, std::int16_t row) noexcept :
            GDSValue(),  _column{column}, _row{row} {}
        ~GDSColumnRow() noexcept {}

        std::int16_t column() const noexcept
        { return this->_column; }

        std::int16_t row() const noexcept
        { return this->_row; }

    private:
        std::int16_t _column;
        std::int16_t _row;
    };

}