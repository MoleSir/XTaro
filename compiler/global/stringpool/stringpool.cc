#include "stringpool.hh"

namespace xtaro
{
    StringPool* stringPool{ StringPool::instance() };

    StringPool* StringPool::instance()
    {
        static StringPool _stringPool;
        return &_stringPool;
    }

    String StringPool::get(const std::string& str)
    {
        auto iter {this->_strings.find(str)};
        if (iter == this->_strings.end())
        {
            auto res = this->_strings.emplace( str );
            const char* cstr { (res.first)->c_str() };
            return String {cstr};
        }

        return iter->c_str();
    }
    
}