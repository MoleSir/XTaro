#pragma once

#include <string>

namespace xtaro
{
    struct PVT
    {
        PVT(std::string p, double v, double t) :
            process{std::move(p)}, voltage{v}, temperature{t} {}

        std::string process;
        double      voltage;
        double      temperature;
    };
}