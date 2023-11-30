#pragma once

#include <cstdint>

namespace xtaro::parser
{

    class GDSColumnRow
    {
    public:
        GDSColumnRow(std::int16_t column, std::int16_t row) :
            _column{column}, _row{row} {}

        std::int16_t column() const noexcept
        { return this->_column; }

        std::int16_t row() const noexcept
        { return this->_row; }

    private:
        std::int16_t _column;
        std::int16_t _row;
    };

}