#include "sram.hh"

#include <module/inputdffs.hh>
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
        _inputDffs{nullptr},
        _bank{nullptr},
        _controllogic{nullptr}
    {
        if (this->_addressWidth < 1 || this->_wordWidth < 1)
        {
            debug->errorWithException("Create SRAM", util::format(
                    "SRAM's address width '%d' or word width '%d' < 1", 
                    this->_addressWidth, this->_wordWidth
            ));
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
            this->addPort(stringFactory->get("addr%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("din%d", i), PortType::INPUT);

        for (int i = 0; i < this->_wordWidth; ++i)
            this->addPort(stringFactory->get("dout%d", i), PortType::OUTPUT);
        
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void SRAM::createCircuits()
    {
        InputDFFsArguments inputArgs {this->_addressWidth, this->_wordWidth};
        this->_inputDffs = this->addCircuit("input_dffs", &inputArgs);

        BankArguments bankArgs {this->_addressWidth, this->_wordWidth};
        this->_bank = this->addCircuit("bank", &bankArgs);

        ControlLogicArguments controlArgs{};
        this->_controllogic = this->addCircuit("control_logic", &controlArgs);
    } 

    void SRAM::createInstances() 
    {
        // Ports
        std::vector<std::string_view> addressRegPorts{};
        std::vector<std::string_view> addressPorts{};
        std::vector<std::string_view> wordRegPorts{};
        std::vector<std::string_view> wordPorts{};

        for (int i = 0; i < this->_addressWidth; ++i)
        {
            addressRegPorts.emplace_back(stringFactory->get("addr%d_r", i));
            addressPorts.emplace_back(stringFactory->get("addr%d_r", i));
        }

        for (int i = 0; i < this->_wordWidth; ++i)
        {
            wordRegPorts.emplace_back(stringFactory->get("din%d_r", i));
            wordPorts.emplace_back(stringFactory->get("din%d_r", i));
        }

        // For each input, use an DFF to save
        std::vector<std::string_view> inputDffsPorts {"clk", "csb", "we"};
        
        for (int i = 0; i < this->_addressWidth; ++i)
            inputDffsPorts.emplace_back(addressRegPorts[i]);

        for (int i = 0; i < this->_wordWidth; ++i)
            inputDffsPorts.emplace_back(wordRegPorts[i]);

        inputDffsPorts.emplace_back("csb_r");
        inputDffsPorts.emplace_back("we_r");

        for (int i = 0; i < this->_addressWidth; ++i)
            inputDffsPorts.emplace_back(addressPorts[i]);

        for (int i = 0; i < this->_wordWidth; ++i)
            inputDffsPorts.emplace_back(wordPorts[i]);

        inputDffsPorts.emplace_back("vdd");
        inputDffsPorts.emplace_back("gnd");

        this->addInstance("input_dffs", this->_inputDffs, inputDffsPorts);

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