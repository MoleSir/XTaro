#include <log/log.hh>
#include <config/config.hh>
#include <tech/tech.hh>
#include <iostream>

using namespace xtaro;

int main()
{
    logger.open("/mnt/e/XTaro/temp/xtrao.log");
    std::cout << "Hello World Xtaro!!" << std::endl;

    config.load("./temp/config.json");

    std::cout << config.wordWidth << std::endl;
    std::cout << config.addressWidth << std::endl;
    std::cout << config.techName << std::endl;
    std::cout << config.techPath << std::endl;
    std::cout << config.outputPath << std::endl;

    tech.load();

    std::cout << tech.spice.nmos << std::endl;
    std::cout << tech.spice.pmos << std::endl;

    return 0;
}