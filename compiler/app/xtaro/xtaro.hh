#pragma once

#include <string>
#include <map>
#include <vector>

namespace xtaro
{

    class XTaro
    {
        enum class Mode 
        {
            NONE,
            HELP,
            VERION,
            INTERACTIVE,
            SCRIPT,
        };

    public:
        static std::vector<std::string> parse(int argc, const char* argv[]);
        static void run(const std::vector<std::string>& arguments);

    private:
        static std::map<std::string, Mode> argvMap;
        static Mode mode;

    private:
        static void runHelpMode(const std::vector<std::string>& arguments);
        static void runVersionMode(const std::vector<std::string>& arguments);
        static void runInteractiveMode(const std::vector<std::string>& arguments);
        static void runScriptMode(const std::vector<std::string>& arguments);
    };

} // namespace xtaro
