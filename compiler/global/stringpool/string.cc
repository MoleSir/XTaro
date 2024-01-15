#include "string.hh"
#include "stringpool.hh"
#include <cstring>
#include <iostream>
#include <string>

namespace xtaro
{

    String::String(const std::string& str) :
        _cstr{nullptr}
    {
        String s {stringPool->get(str)};
        this->_cstr = s.cstr();
    }

    String::String(const char* cstr) : 
        _cstr{cstr}
    {
    }

    String::~String() noexcept
    {
    }

    String::String(const String& that) : 
        _cstr{that._cstr}
    {
    }
    
    String::String(String&& that) :
        _cstr{that._cstr}
    {
    }

    bool String::empty() const noexcept
    {
        return strlen(this->_cstr) == 0;
    }

    String& String::operator = (const String& that)
    {
        this->_cstr = that._cstr;
        return *this;
    }

    String& String::operator = (String&& that)
    {
        this->_cstr = that._cstr;
        return *this;
    }

    std::ostream& operator << (std::ostream& out, const String& string)
    {
        out << string.cstr();
        return out;
    }

    bool operator == (const String& string1, const String& string2)
    {
        return std::strcmp(string1.cstr(), string2.cstr()) == 0;
    }

    bool operator != (const String& string1, const String& string2)
    {
        return !(string1 == string2);
    }

    bool operator >  (const String& string1, const String& string2)
    {
        return std::strcmp(string1.cstr(), string2.cstr()) >  0;
    }

    bool operator <  (const String& string1, const String& string2)
    {
        return std::strcmp(string1.cstr(), string2.cstr()) <  0;
    }

    bool operator >= (const String& string1, const String& string2)
    {
        return (string1 > string2) && (string1 == string2);
    }

    bool operator <= (const String& string1, const String& string2)
    {
        return (string1 < string2) && (string1 == string2);
    }

}