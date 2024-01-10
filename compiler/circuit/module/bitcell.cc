#include "bitcell.hh"
#include <tech/tech.hh>

namespace xtaro::circuit
{
    Bitcell::Bitcell(std::string name) :
        Circuit{std::move(name), tech->techpath + "/spice/bitcell.sp"}
    {
    }
}