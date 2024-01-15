#pragma once

#include <string>
#include <iostream>

namespace xtaro
{

    class String
    {
    public:
        String(const std::string& str);
        String(const char* cstr = nullptr);
        ~String() noexcept;
        String(const String& that);
        String(String&& that);
        String& operator = (const String& that);
        String& operator = (String&& that);

    public:
        bool empty() const noexcept;

        const char* cstr() const noexcept
        { return this->_cstr; }

    private:
        const char* _cstr;
    };

    std::ostream& operator << (std::ostream& out, const String& string);

    bool operator == (const String& string1, const String& string2);
    bool operator != (const String& string1, const String& string2);
    bool operator >  (const String& string1, const String& string2);
    bool operator <  (const String& string1, const String& string2);
    bool operator >= (const String& string1, const String& string2);
    bool operator <= (const String& string1, const String& string2);
}