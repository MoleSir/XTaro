#include "inputdffs.hh"
#include <base/circuit.hh>
#include <config/tech.hh>
#include <debug/debug.hh>
#include <util/format.hh>
#include <factory/stringfactory.hh>

namespace xtaro::circuit
{
 
    std::string InputDFFsArguments::toString() const
    {
        return util::format("aw%d_ww%d", this->addressWidth, this->wordWidth);
    }

    InputDFFs::InputDFFs(const std::string_view& name, InputDFFsArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressWidth{arguments->addressWidth},
        _wordWidth{arguments->wordWidth}
    {
        if (this->_addressWidth < 1 || this->_wordWidth < 1)
        {
            debug->errorWithException("Create Input DFFs", util::format(
                    "Input DFFs' address width '%d' or word width '%d' < 1", 
                    this->_addressWidth, this->_wordWidth
            ));
        }

        debug->debug("Create a 'Input DFFs' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void InputDFFs::createPorts()
    {
        this->addPort("clk", PortType::INPUT);
        this->addPort("csb", PortType::INPUT);
        this->addPort("we", PortType::INPUT);

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addPort(stringFactory->get("addr%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("din%d", i), PortType::INPUT);

        this->addPort("csb_r", PortType::INPUT);
        this->addPort("we_r", PortType::INPUT);

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addPort(stringFactory->get("addr%d_r", i), PortType::OUTPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("din%d_r", i), PortType::OUTPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void InputDFFs::createCircuits()
    {
        this->_dff = this->addCircuit("dff", nullptr);
    } 

    void InputDFFs::createInstances() 
    {
        this->addInstance("csb_dff", this->_dff, {"csb", "csb_r", "clk", "vdd", "gnd"});
        this->addInstance("we_dff", this->_dff, {"we", "we_r", "clk", "vdd", "gnd"});

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addInstance(
                stringFactory->get("addr%d_dff", i), this->_dff, 
                {
                    stringFactory->get("addr%d", i), 
                    stringFactory->get("addr%d_r", i), 
                    "clk", "vdd", "gnd"
                }
            );
    
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addInstance(
                stringFactory->get("din%d", i), this->_dff, 
                {
                    stringFactory->get("din%d", i), 
                    stringFactory->get("din%d_r", i),
                    "clk", "vdd", "gnd"
                }
            );
    }
}