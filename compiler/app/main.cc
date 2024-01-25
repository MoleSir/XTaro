#include <xtaro/xtaro.hh>

using namespace xtaro;

int main(int argc, const char* argv[])
{
    auto arguments {XTaro::parse(argc, argv)};
    XTaro::run(arguments);
    return 0;
}