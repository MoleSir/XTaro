#include "exception.hh"

#include <util/format.hh>

#include <exception>
#include <string>

namespace xtaro
{

    MessageException::MessageException(std::string message) :
        _message{ std::move(message) }
    {
    }

    const char* MessageException::what() const noexcept 
    {
        return this->_message.c_str();   
    }


    TypeReasonException::TypeReasonException(
                         const std::string& exceptionType, 
                         const std::string& exceptionReason) :
        _message{ util::format("[%s] exception! Because [%s]", 
                                exceptionType.c_str(), 
                                exceptionReason.c_str()) }
    {
    }

    const char* TypeReasonException::what() const noexcept 
    {
        return this->_message.c_str();   
    }

}