#pragma once

#include <cstdint>

namespace xtaro::parser
{

    struct GDSDateTime
    {
        GDSDateTime(std::int16_t y, std::int16_t m,  std::int16_t d, 
                    std::int16_t h, std::int16_t mi, std::int16_t s) :
            year{y}, month{m}, day{d}, hour{h}, minute{mi}, second{s} {}
        
        std::int16_t year, month, day;
        std::int16_t hour, minute, second;
    };

    class GDSTimestamp
    {
    public:
        GDSTimestamp(std::int16_t cy, std::int16_t cm,  std::int16_t cd, 
                     std::int16_t ch, std::int16_t cmi, std::int16_t cs,
                     std::int16_t my, std::int16_t mm,  std::int16_t md, 
                     std::int16_t mh, std::int16_t mmi, std::int16_t ms) :
            _createDateTime{cy, cm, cd, ch, cmi, cs}, 
            _modifyDateTime{my, mm, md, mh, mmi, ms}  
        {
        }

        const GDSDateTime& createDateTime() const noexcept
        { return this->_createDateTime; }

        const GDSDateTime& modifyDateTime() const noexcept
        { return this->_modifyDateTime; }

    private:
        GDSDateTime _createDateTime;
        GDSDateTime _modifyDateTime;
    };

}