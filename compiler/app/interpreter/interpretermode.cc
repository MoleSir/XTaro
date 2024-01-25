#include <xtaro/xtaro.hh>
#include <iostream>

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
            if (command == "exit")
                break;

            std::cout << command << std::endl;   
        }
    }

}