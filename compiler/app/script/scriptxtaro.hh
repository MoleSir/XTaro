#pragma once

#include <string>
#include <memory>
#include <vector>

namespace xtaro
{

    struct PVT;
    namespace circuit {
        class SRAM;
    }

    class ScriptXTaro
    {
    public:
        static ScriptXTaro* instance();

    public:
        void init(const std::string& optionFile);
        void createSRAM();
        void saveFiles();

    private:
        std::vector<PVT> pvtLists();

    private:
        ScriptXTaro() = default;
        ~ScriptXTaro() noexcept = default;
        ScriptXTaro(const ScriptXTaro&) = delete;
        ScriptXTaro(ScriptXTaro&&) = delete;
        ScriptXTaro& operator = (const ScriptXTaro&) = delete;
        ScriptXTaro& operator = (ScriptXTaro&&) = delete;

    private:
        std::unique_ptr<circuit::SRAM> _sram{nullptr};
    };

}