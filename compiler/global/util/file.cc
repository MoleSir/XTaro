#include "file.hh"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace xtaro::util
{
    std::size_t filesize(std::ifstream& file)
    {
        file.seekg(0, std::ios::end);
        std::size_t size{ static_cast<std::size_t>(file.tellg()) };
        file.seekg(0, std::ios::beg);
        return size;
    }

    std::string readFile(std::ifstream& file)
    {
        std::size_t size {filesize(file)};
        std::string content {};
        content.resize(size + 1);
        file.read(content.data(), size);
        content.resize(size);
        return content;
    }

    bool fileExists(const std::string& filePath) 
    {
        return std::filesystem::exists(filePath) && 
               std::filesystem::is_regular_file(filePath);
    }

    bool directoryExists(const std::string& dirPath) 
    {
        return std::filesystem::exists(dirPath) && 
               std::filesystem::is_directory(dirPath);
    }

    std::string absolutePath(const std::string& relatePath)
    {
        return std::filesystem::absolute(relatePath);
    }
}