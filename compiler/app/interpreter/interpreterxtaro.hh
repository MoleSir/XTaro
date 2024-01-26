#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>

namespace xtaro
{

    namespace circuit {
        class SRAM;
    }

    class InterpreterExit {};

    class InterpreterXTaro
    {
        enum class Command
        {
            NONE,
            LOAD_OPTION,
            COMPILE,
            FUNCTION_TEST,
            EXIT,
        };

    public:
        static InterpreterXTaro* instance();

    public:
        void interprete(std::string line);

    private:
        bool parse(std::string commandLine);
        void execute();

    private:
        std::vector<std::string> splitCommandLine(const std::string& commandLine);

    private:
        void loadOption();
        void compile();
        void functionTest();
        void exit();

    private:
        InterpreterXTaro() = default;
        ~InterpreterXTaro() noexcept = default;
        InterpreterXTaro(const InterpreterXTaro&) = delete;
        InterpreterXTaro(InterpreterXTaro&&) = delete;
        InterpreterXTaro& operator = (const InterpreterXTaro&) = delete;
        InterpreterXTaro& operator = (InterpreterXTaro&&) = delete;

    private:
        static std::map<std::string, Command> commandMap;

    private:
        std::vector<std::string> _historyCommands{};
        Command _command{Command::NONE};
        std::vector<std::string> _arguments{};
    };

}