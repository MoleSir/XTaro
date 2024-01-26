#include <xtaro/xtaro.hh>
#include "interpreterxtaro.hh"
#include <iostream>

namespace xtaro 
{

    void XTaro::runInterpreterMode(const std::vector<std::string>& arguments)
    {
        InterpreterXTaro* xtaro {InterpreterXTaro::instance()};
        while (true)
        {
            std::cout << "XTaro> ";
            std::string line;
            std::getline(std::cin, line);

            try
            {
                xtaro->interprete(std::move(line));
            }
            catch (const std::exception& err) {}
            catch (const InterpreterExit& err)
            {
                std::cout << "Exit from interpreter mode" << std::endl;
                break;
            }
        }
    }

}