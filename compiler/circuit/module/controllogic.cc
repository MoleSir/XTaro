#include "controllogic.hh"

#include <module/inv.hh>
#include <module/or.hh>
#include <module/nor.hh>
#include <module/nand.hh>

#include <factory/stringfactory.hh>

#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string ControlLogicArguments::toString() const
    {
        return "_";
    }

    ControlLogic::ControlLogic(const std::string_view& name, ControlLogicArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _inv{nullptr},
        _powerInv{nullptr},
        _or2{nullptr},
        _nor2{nullptr},
        _nor3{nullptr},
        _nand2{nullptr}
    {
        debug->debug("Create a 'Control Logic' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void ControlLogic::createPorts()
    {
        this->addPort("clk", PortType::INPUT);
        this->addPort("csb", PortType::INPUT);
        this->addPort("we", PortType::INPUT);
        this->addPort("rbl", PortType::INPUT);

        this->addPort("wl_en", PortType::OUTPUT);
        this->addPort("p_en_bar", PortType::OUTPUT);
        this->addPort("sa_en", PortType::OUTPUT);
        this->addPort("we_en", PortType::OUTPUT);

        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void ControlLogic::createCircuits()
    {
        INVArguments invArgs{DriveCapability::NORMAL};
        this->_inv = this->addCircuit("inv", &invArgs);

        invArgs.driveCapability = DriveCapability::STRONG_2;
        this->_powerInv = this->addCircuit("inv", &invArgs);

        ORArguments orArgs{DriveCapability::NORMAL, 2};
        this->_or2 = this->addCircuit("or", &orArgs);

        NORArguments norArgs{DriveCapability::NORMAL, 2};
        this->_nor2 = this->addCircuit("nor", &norArgs);
        norArgs.inputSize = 3;
        this->_nor3 = this->addCircuit("nor", &norArgs);

        NANDArguments nandArgs{DriveCapability::NORMAL, 2};
        this->_nand2 = this->addCircuit("nand", &nandArgs);
    }

    void ControlLogic::createInstances() 
    {
        // TODO clk ==> iclk
        this->addInstance("inv0", this->_inv, {"clk", "clk_bar", "vdd", "gnd"});
        this->addInstance("inv1", this->_inv, {"clk_bar", "iclk", "vdd", "gnd"});

        // Ecah input signals INV
        this->addInstance("csbInv", this->_inv, {"csb", "csb_bar", "vdd", "gnd"});
        this->addInstance("weInv", this->_inv, {"we", "we_bar", "vdd", "gnd"});
        this->addInstance("rblInv", this->_inv, {"rbl", "rbl_bar", "vdd", "gnd"});
        this->addInstance("iclkInv", this->_inv, {"iclk", "iclk_bar", "vdd", "gnd"});

        // Generate wl_en
        this->addInstance("wl_en_or2", this->_or2, {"we", "rbl", "wl_net0", "vdd", "gnd"});
        this->addInstance("wl_en_nand2", this->_nand2, {"iclk", "wl_net0", "wl_en_bar", "vdd", "gnd"});
        this->addInstance("wl_en_inv", this->_powerInv, {"wl_en_bar", "wl_en", "vdd", "gnd"});

        // Generate sa_en
        this->addInstance("sa_en_nor2_0", this->_nor2, {"we", "rbl", "sa_en_net0", "vdd", "gnd"});
        this->addInstance("sa_en_nor2_1", this->_nor2, {"csb", "iclk_bar", "sa_en_net1", "vdd", "gnd"});
        this->addInstance("sa_en_nand2", this->_nand2, {"sa_en_net0", "sa_en_net1", "sa_en_bar", "vdd", "gnd"});
        this->addInstance("sa_en_inv", this->_powerInv, {"sa_en_bar", "sa_en", "vdd", "gnd"});

        // Generate p_bar_en
        this->addInstance("p_en_bar_nor3", this->_nor3, {"csb", "we", "iclk", "p_en", "vdd", "gnd"});
        this->addInstance("p_en_bar_inv", this->_powerInv, {"p_en", "p_en_bar", "vdd", "gnd"});

        // Generate we_en
        this->addInstance("we_en_nand2", this->_nand2, {"csb_bar", "we", "we_en_bar", "vdd", "gnd"});
        this->addInstance("we_en_inv", this->_powerInv, {"we_en_bar", "we_en", "vdd", "gnd"});
    }
}