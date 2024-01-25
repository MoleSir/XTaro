#pragma once

#include <string>
#include <memory>

namespace xtaro
{

    namespace circuit {
        class SRAM;
    }

    class ScriptXtaro
    {
    public:
        static ScriptXtaro* instance();

    public:
        void init(const std::string& optionFile);
        void createSRAM();
        void saveFiles();

    private:
        ScriptXtaro() = default;
        ~ScriptXtaro() noexcept = default;
        ScriptXtaro(const ScriptXtaro&) = delete;
        ScriptXtaro(ScriptXtaro&&) = delete;
        ScriptXtaro& operator = (const ScriptXtaro&) = delete;
        ScriptXtaro& operator = (ScriptXtaro&&) = delete;

    private:
        std::unique_ptr<circuit::SRAM> _sram{nullptr};
    };

}