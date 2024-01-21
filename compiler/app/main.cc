#include "xtaro/xtaro.hh"

using namespace xtaro;

int main(int argc, const char* argv[])
{
    xTaro->run(xTaro->parseArgv(argc, argv));
    return 0;
}