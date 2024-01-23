#include <exception>
#include <string>

namespace xtaro
{
    class MessageException : public std::exception 
    {
    public:
        MessageException(std::string message);

        virtual const char* what() const noexcept override;

    private:
        std::string _message;
    };

    class TypeReasonException : public std::exception 
    {
    public:
        TypeReasonException(
            const std::string& exceptionType, 
            const std::string& exceptionReason);

        virtual const char* what() const noexcept override;

    private:
        std::string _message;
    };
}