#include "bank.hh"

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string XXXArguments::toString() const
    {
        return util::format("");
    }

    XXX::XXX(std::string name, XXXArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT}
    {
        this->createNetlist();
    }

    void XXX::createPorts()
    {
    }

    void XXX::createCircuits()
    {   
    } 

    void XXX::createInstances() 
    {
    }
}