#pragma once

#include <cstdint>

namespace xtaro::parser 
{

    class GDSElementFlags 
    {
    public:
        union FlagsValue
        {
            FlagsValue(std::uint16_t v) : value{v} {}
            std::uint16_t value;
            struct
            {
                unsigned unused : 14;
                unsigned isExternal : 1;
                unsigned isTemplate : 1;
            };
        };
        
    public:
        GDSElementFlags(std::uint16_t value = 0) : _value{value} {}

        // Getter
        bool isExternal() const noexcept 
        { return this->_value.isExternal; }

        bool isTemplate() const noexcept 
        { return this->_value.isTemplate; }

        FlagsValue get_value() const noexcept
        { return this->_value; }

        // Setter
        void setIsExternal(bool enabled) noexcept
        { this->_value.isExternal = enabled; }
        
        void setIsTemplate(bool enabled) noexcept
        { this->_value.isTemplate = enabled; }

    private:
        FlagsValue _value;
    };

}
