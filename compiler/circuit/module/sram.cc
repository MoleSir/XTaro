#include "sram.hh"

#include <module/dff.hh>
#include <module/bank.hh>
#include <module/controllogic.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string SRAMArguments::toString() const
    {
        return util::format("aw%d_ww%d", this->addressWidth, this->wordWidth);
    }

    SRAM::SRAM(const std::string_view& name, SRAMArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _addressWidth{arguments->addressWidth},
        _wordWidth{arguments->wordWidth},
        _dff{nullptr},
        _bank{nullptr},
        _controllogic{nullptr}
    {
        if (this->_addressWidth < 1 || this->_wordWidth < 1)
        {
            std::string errorMsg {
                util::format(
                    "SRAM's address width '%d' or word width '%d' < 1", 
                    this->_addressWidth, this->_wordWidth
                )
            };

            debug->errorWithException("Create SRAM", errorMsg);
        }

        debug->debug("Create a 'SRAM' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void SRAM::createPorts()
    {
        this->addPort("clk", PortType::INPUT);
        this->addPort("csb", PortType::INPUT);
        this->addPort("we", PortType::INPUT);

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addPort(stringFactory->get("A%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("D%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("dout%d", i), PortType::OUTPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void SRAM::createCircuits()
    {
        this->_dff = this->addCircuit("dff", nullptr);

        BankArguments bankArgs{this->_addressWidth, this->_wordWidth};
        this->_bank = this->addCircuit("bank", &bankArgs);

        ControlLogicArguments controlArgs{};
        this->_controllogic = this->addCircuit("control_logic", &controlArgs);
    } 

    void SRAM::createInstances() 
    {
        // Ports
        std::vector<std::string_view> addressRegPorts{};
        std::vector<std::string_view> wordRegPorts{};

        for (int i = 0; i < this->_addressWidth; ++i)
            addressRegPorts.emplace_back(stringFactory->get("A%d_r", i));

        for (int i = 0; i < this->_wordWidth; ++i)
            wordRegPorts.emplace_back(stringFactory->get("D%d_r", i));

        // For each input, use an DFF to save
        this->addInstance("csb_dff", this->_dff, {"csb", "csb_r", "clk", "vdd", "gnd"});
        this->addInstance("we_dff", this->_dff, {"we", "we_r", "clk", "vdd", "gnd"});

        for (int i = 0; i < this->_addressWidth; ++i)
            this->addInstance(
                stringFactory->get("A%d_dff", i), this->_dff, 
                {stringFactory->get("A%d", i), addressRegPorts[i], "clk", "vdd", "gnd"}
            );
    
        for (int i = 0; i < this->_wordWidth; ++i)
            this->addInstance(
                stringFactory->get("D%d", i), this->_dff, 
                {stringFactory->get("D%d", i), wordRegPorts[i], "clk", "vdd", "gnd"}
            );

        // Bank
        Instance* bank {this->addInstance("bank", this->_bank)};

        std::vector<std::string_view> bankPorts{};
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
            bankPorts.emplace_back(stringFactory->get("dout%d", i));
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