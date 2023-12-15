#pragma once

#include <cstdint>
#include <string>

namespace xtaro::parser
{

    class GDSValue {};

    class GDSInt16 : public GDSValue
    {
    public:
        GDSInt16(std::int16_t value) noexcept :
            GDSValue(), _value{value} {}
        ~GDSInt16() noexcept {}

        std::int16_t value() const noexcept
        { return this->_value; }

        void setValue(std::int16_t v) noexcept
        { this->_value = v; }
    
    private:
        std::int16_t _value;
    };

    class GDSInt32 : public GDSValue
    {
    public:
        GDSInt32(std::int32_t value) noexcept :
            GDSValue(), _value{value} {}
        ~GDSInt32() noexcept {}

        std::int32_t value() const noexcept
        { return this->_value; }

        void setValue(std::int32_t v) noexcept
        { this->_value = v; }
    
    private:
        std::int32_t _value;
    };

    class GDSString : public GDSValue
    {
    public:
        GDSString(const char* value) :
            GDSValue(), _value{value} {}
        ~GDSString() noexcept {}

        const std::string& value() const noexcept
        { return this->_value; }
    
    private:
        std::string _value;
    };

}