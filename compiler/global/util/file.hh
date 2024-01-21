#pragma once

#include <string>
#include <fstream>

namespace xtaro::util
{
    std::size_t filesize(std::ifstream& file);
    std::string readFile(std::ifstream& file);
    bool fileExists(const std::string& filePath);
    bool directoryExists(const std::string& dirPath);
    std::string absolutePath(const std::string& relatePath);
}