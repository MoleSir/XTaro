#include "xtaro.hh"

#include <config/option.hh>
#include <config/tech.hh>
#include <debug/logger.hh>

#include <factory/circuitfactory.hh>
#include <module/sram.hh>
#include <verilog/verilog.hh>
#include <character/function.hh>

namespace xtaro
{

    void XTaro::runInterpreterMode(const std::vector<std::string>& arguments)
    {
        while (true)
        {
            std::cout << "XTaro> ";
            std::string command;
            std::getline(std::cin, command);

            // Parse command

            std::cout << command << std::endl;   
        }
    }

}
