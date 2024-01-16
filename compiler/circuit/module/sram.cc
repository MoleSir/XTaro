#include "sram.hh"

#include <module/dff.hh>
#include <module/bank.hh>
#include <module/controllogic.hh>

#include <factory/circuitfactory.hh>
#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string SRAMArguments::toString() const
    {
        return util::format("aw%d_ww%d", this->addressWidth, this->wordWidth);
    }

    SRAM::SRAM(String name, SRAMArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressWidth{arguments->addressWidth},
        _wordWidth{arguments->wordWidth},
        _dff{nullptr},
        _bank{nullptr},
        _controllogic{nullptr}
    {
        this->createNetlist();
    }

    void SRAM::createPorts()
    {
        this->addPort("clk", PortType::INPUT);
        this->addPort("csb", PortType::INPUT);
        this->addPort("we", PortType::INPUT);

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("D%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(util::format("dout%d", i), PortType::OUTPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void SRAM::createCircuits()
    {
        this->_dff = factory->create(ModuleType::DFF, nullptr);
        this->_circuits.emplace(this->_dff);

        BankArguments bankArgs{this->_addressWidth, this->_wordWidth};
        this->_bank = factory->create(ModuleType::BANK, &bankArgs);
        this->_circuits.emplace(this->_bank);

        ControlLogicArguments controlArgs{};
        this->_controllogic = factory->create(ModuleType::CONTROL_LOGIC, &controlArgs);
        this->_circuits.emplace(this->_controllogic);
    } 

    void SRAM::createInstances() 
    {
        // Ports
        std::vector<String> addressRegPorts{};
        std::vector<String> wordRegPorts{};

        for (int i = 0; i < this->_addressWidth; ++i)
            addressRegPorts.emplace_back(util::format("A%d_r", i));

        for (int i = 0; i < this->_wordWidth; ++i)
            wordRegPorts.emplace_back(util::format("D%d_r", i));

        // For each input, use an DFF to save
        this->addInstance("csb_dff", this->_dff, {"csb", "csb_r", "clk", "vdd", "gnd"});
        this->addInstance("we_dff", this->_dff, {"we", "we_r", "clk", "vdd", "gnd"});

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addInstance("csb_dff", this->_dff, {
                util::format("A%d", i), addressRegPorts[i], "clk", "vdd", "gnd"
            });
    
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addInstance("csb_dff", this->_dff, {
                util::format("D%d", i), wordRegPorts[i], "clk", "vdd", "gnd"
            });

        // Bank
        Instance* bank {this->addInstance("bank", this->_bank)};

        std::vector<String> bankPorts{};
        for (int i = 0; i < this->_addressWidth; ++i)
            bankPorts.emplace_back(addressRegPorts[i]);
        for (int i = 0; i < this->_wordWidth; ++i)
            bankPorts.emplace_back(wordRegPorts[i]);
        bankPorts.emplace_back("wl_en");
        bankPorts.emplace_back("p_en_bar");
        bankPorts.emplace_back("sa_en");
        bankPorts.emplace_back("we_en");
        bankPorts.emplace_back("rbl");
        for (int i = 0; i < this->_wordWidth; ++i)
            bankPorts.emplace_back(util::format("dout%d", i));
        bankPorts.emplace_back("vdd");
        bankPorts.emplace_back("gnd");
        this->connectWith(bank, bankPorts);

        // Control logic
        Instance* control {this->addInstance("control_logic", this->_controllogic)};
        this->connectWith(control, {
            "clk", "csb", "we_r", "rbl", 
            "wl_en", "p_en_bar", "sa_en", "we_en", 
            "vdd", "gnd"
        });
    }
}