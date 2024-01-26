#pragma once

#include <string>

namespace xtaro
{
    struct PVT
    {
        PVT(std::string p, double v, double t) :
            process{std::move(p)}, voltage{v}, temperature{t} {}
        ~PVT() noexcept = default;
        PVT(const PVT&) = default;
        PVT(PVT&&) noexcept = default;
        PVT& operator = (const PVT&) = default;
        PVT& operator = (PVT&&) noexcept = default;

        std::string process;
        double      voltage;
        double      temperature;
    };
}