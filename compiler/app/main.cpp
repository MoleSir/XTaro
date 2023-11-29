#include <iostream>
#include "../global/global.hh"

using namespace xtaro;

int main()
{
    std::cout << "Hello XTaro!" << std::endl;

    logger->open("./temp/text.log");

    logger->debug("hello %d %f %s", 1, 22.22, "hhh");
    logger->info("some info");
    logger->warning("!!!!!!!!!!!");
    logger->error("eoeoeoee");
    logger->fatal("bad bad");

    return 0;
}