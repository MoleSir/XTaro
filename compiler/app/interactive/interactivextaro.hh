#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>
#include <list>

namespace xtaro
{

    namespace circuit {
        class SRAM;
    }

    class InteractiveExit {};

    class InteractiveXTaro
    {
        using CommandMethod = std::function<void(void)>; 

    public:
        static InteractiveXTaro* instance();
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

        void setDebugLevel();

        void logo();

    private:
        bool checkOption() const;
        bool checkCompile() const;

        void clearCurrentInput();
        
    private:
        InteractiveXTaro();
        ~InteractiveXTaro() noexcept = default;
        InteractiveXTaro(const InteractiveXTaro&) = delete;
        InteractiveXTaro(InteractiveXTaro&&) noexcept = delete;
        InteractiveXTaro& operator = (const InteractiveXTaro&) = delete;
        InteractiveXTaro& operator = (InteractiveXTaro&&) noexcept = delete;

    private:
        std::map<std::string, CommandMethod> _methodMap{};

        std::string _commandLine{};
        std::list<std::string> _historyCommands{"", ""};

        CommandMethod _method{nullptr};
        std::vector<std::string> _arguments{};

        bool _loadOptionSucc{false};
        bool _compileSucc{false};
        std::unique_ptr<circuit::SRAM> _sram{nullptr};
    };

}