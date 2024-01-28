#include "stringfactory.hh"
#include <util/format.hh>
#include <cstdarg>

namespace xtaro::circuit
{

    StringFactory* stringFactory {StringFactory::instance()};

    StringFactory* StringFactory::instance()
    {
        static StringFactory _factory;
        return &_factory;
    }

    std::string_view 
    StringFactory::get(const char* fmt, ...)
    {
        va_list(args);
        va_start(args, fmt);
        std::string str {util::vformat(fmt, args)};
        va_end(args);

        return this->get(std::move(str));
    }

    std::string_view 
    StringFactory::get(std::string str)
    {
        // auto iter {this->_stringPool.find(str)};
        // if (iter != this->_stringPool.end())
        //     return std::string_view{*iter};
        
        // std::string_view view {str};
        // auto res {this->_stringPool.emplace(std::move(str))};
        // res.
        // return view;

        auto iter {this->_stringPool.find(str)};
        if (iter == this->_stringPool.end())
        {
            auto res = this->_stringPool.emplace( str );
            const char* cstr { (res.first)->c_str() };
            return std::string_view {cstr};
        }
        return std::string_view {iter->c_str()};
    }

    std::string_view 
    StringFactory::get(const char* str)
    {
        // C-style strings are stored in static area
        return std::string_view {str};
    }

}