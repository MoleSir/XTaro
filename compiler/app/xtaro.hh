#pragma once

#include <string>

namespace xtaro
{
    void init(const std::string& configfile);
    void createSRAM();
    void saveFiles();
} // namespace xtaro
