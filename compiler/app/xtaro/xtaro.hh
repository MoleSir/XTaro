#pragma once

#include <string>
#include <map>
#include <vector>

namespace xtaro
{
    namespace circuit {
        class SRAM;
    }

    enum class RunMode
    {
        NONE,
        HELP,
        VERION,
        INTERPRETER,
        SCRIPT,
    };

    enum class ErrorCode
    {
        
    };

    class XTaro
    {
    public:
        static XTaro* instance();
        std::vector<std::string> parseArgv(int argc, const char* argv[]);
        void run(const std::vector<std::string>& arguments);
    
    private:
        void runHelpMode(const std::vector<std::string>& arguments);
        void runVersionMode(const std::vector<std::string>& arguments);
        void runInterpreterMode(const std::vector<std::string>& arguments);
        void runScriptMode(const std::vector<std::string>& arguments);

    private:
        

    private:
        void init(const std::string& optionFile);
        void createSRAM();
        void saveFiles();
    
    private:
        XTaro() = default;
        ~XTaro() noexcept = default;
        XTaro(const XTaro&) = delete;
        XTaro(XTaro&&) = delete;
        XTaro& operator = (const XTaro&) = delete;
        XTaro& operator = (XTaro&&) = delete;

    private:
        circuit::SRAM* _sram{nullptr};
        RunMode _runMode{RunMode::NONE};

        static std::map<std::string, RunMode> argvMap;
        static const char* helpMessage;
        static const char* verionMessage;
    };

    extern XTaro* xTaro;

} // namespace xtaro
