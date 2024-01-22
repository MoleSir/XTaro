#include "xtaro/xtaro.hh"
#include <debug/console.hh>

using namespace xtaro;

int main(int argc, const char* argv[])
{
    xTaro->run(xTaro->parseArgv(argc, argv));
    return 0;
}