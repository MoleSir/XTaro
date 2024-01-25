#pragma once

#include <string>
#include <memory>

namespace xtaro
{

    namespace circuit {
        class SRAM;
    }

    class InterpreterXTaro
    {
    public:
        static InterpreterXTaro* instance();

    public:

    private:
        InterpreterXTaro() = default;
        ~InterpreterXTaro() noexcept = default;
        InterpreterXTaro(const InterpreterXTaro&) = delete;
        InterpreterXTaro(InterpreterXTaro&&) = delete;
        InterpreterXTaro& operator = (const InterpreterXTaro&) = delete;
        InterpreterXTaro& operator = (InterpreterXTaro&&) = delete;

    private:
    };

}