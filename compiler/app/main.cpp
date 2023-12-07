#include <iostream>
#include "../global/global.hh"
#include "../character/setuphold.hh"

#include "../parse/json/json.hh"

using namespace xtaro;

int main()
{
    logger->open("/mnt/e/XTaro/temp/xtrao.log");

    // character::SetupHoldCharacterizer sh("/mnt/e/XTaro/temp/dff.sp", 10);
    // std::cout << sh.getSetupLHTime() << '\n';

    parse::Json json = parse::Json::loadFromFile("./temp/config.json");
    json.writeToFile("./temp/copy.json");

    return 0;
}