#include "../util/util.hh"

#include <exception>
#include <string>

namespace xtaro
{
    class MessageException : public std::exception 
    {
    public:
        MessageException(const std::string& exceptionType, 
                         const std::string& exceptionReason) :
            _message{ util::format("[%s] exception! Because [%s]", 
                                    exceptionType.c_str(), 
                                    exceptionReason.c_str()) }
        {
        }

        virtual const char* what() const noexcept override 
        {
            return this->_message.c_str();   
        }

    private:
        std::string _message;
    };
}