#pragma once

#include <string>

namespace xtaro
{
    struct PVT
    {
        PVT(std::string p, double v, double t) :
            process{std::move(p)}, voltage{v}, temperature{t} {}
        PVT(const PVT&) = default;
        PVT(PVT&&)=default;
        PVT& operator = (const PVT&) = default;
        PVT& operator = (PVT&&) = default;

        std::string process;
        double      voltage;
        double      temperature;
    };
}