#pragma once

#include <string>

namespace xtaro
{
    class Tech 
    {
    public:
        struct 
        {
            std::string nmos{};
            std::string pmos{};
        } spice;

        void load();

    public:
        static Tech& instance();

    private:
        Tech();
        ~Tech() noexcept = default;
        Tech(const Tech&) = delete;
        Tech(Tech&&) = delete;
        Tech& operator = (const Tech&) = delete;
        Tech& operator = (Tech&&) = delete;
    };

    extern Tech& tech;
}
