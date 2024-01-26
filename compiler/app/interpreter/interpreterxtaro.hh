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
        using CommandMethod = std::function<void(void)>; 

    public:
        static InterpreterXTaro* instance();
        void interprete();
        void getCommandLine();

    private:
        bool parse();
        void execute();

    private:
        std::vector<std::string> splitCommandLine();

    private:
        void loadOption();
        void catOption();

        void compile();
        
        void save();
        void saveSpice();
        void saveVerilog();
        
        void functionTest();
        
        void exit();
        void clear();

    private:
        bool checkOption() const;
        bool checkCompile() const;

        void clearCurrentInput();
        
    private:
        InterpreterXTaro();
        ~InterpreterXTaro() noexcept = default;
        InterpreterXTaro(const InterpreterXTaro&) = delete;
        InterpreterXTaro(InterpreterXTaro&&) noexcept = delete;
        InterpreterXTaro& operator = (const InterpreterXTaro&) = delete;
        InterpreterXTaro& operator = (InterpreterXTaro&&) noexcept = delete;

    private:
        std::map<std::string, CommandMethod> _methodMap{};

        std::string _commandLine{};
        std::vector<std::string> _historyCommands{"clear"};

        CommandMethod _method{nullptr};
        std::vector<std::string> _arguments{};

        bool _loadOptionSucc{false};
        bool _compileSucc{false};
        std::unique_ptr<circuit::SRAM> _sram{nullptr};
    };

}