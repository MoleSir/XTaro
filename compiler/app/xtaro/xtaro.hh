#pragma once

#include <string>

namespace xtaro
{
    namespace circuit {
        class SRAM;
    }

    class XTaro
    {
    public:
        static XTaro* instance();

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
    };

    extern XTaro* xTaro;

} // namespace xtaro
