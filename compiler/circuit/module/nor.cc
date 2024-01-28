#include "nor.hh"

#include <module/mos.hh>

#include <factory/stringfactory.hh>

#include <util/format.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{
    std::string NORArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    NOR::NOR(const std::string_view& name, NORArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nmos{nullptr},
        _pmos{nullptr}
    {
        if (this->_inputSize < 2)
        {
            std::string errorMsg {util::format("NOR gate's input size '%d' < 2", this->_inputSize)};
            debug->errorWithException("Create NOR", errorMsg);
        }

        debug->debug("Create a 'NOR' circuit: '%s'", this->_name.data());
        this->createNetlist();
    }

    void NOR::createPorts()
    {
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(stringFactory->get("A%d", i), PortType::INPUT);

        this->addPort("Z", PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void NOR::createCircuits()
    {
        double length{ tech->drc["minwidth_poly"].asDecimal() };

        // Assign arguments
        MOSArguments nmosArguments{MOSType::NMOS, 0.0, length};
        MOSArguments pmosArguments{MOSType::PMOS, 0.0, length};

        switch (this->_driveCapability)
        {
        case DriveCapability::WEEK:
            nmosArguments.width = length;
            pmosArguments.width = length;
            break;
        case DriveCapability::NORMAL:
            nmosArguments.width = 2.0 * length;
            pmosArguments.width = 4.0 * length;
            break;
        case DriveCapability::STRONG_1:
            nmosArguments.width = 4.0 * length;
            pmosArguments.width = 8.0 * length;
            break;
        case DriveCapability::STRONG_2:
            nmosArguments.width = 6.0 * length;
            pmosArguments.width = 12.0 * length;
            break;
        case DriveCapability::STRONG_3:
            nmosArguments.width = 8.0 * length;
            pmosArguments.width = 16.0 * length;
            break;
        default:
            nmosArguments.width = 2.0 * length;
            pmosArguments.width = 4.0 * length;
            break;
            break;
        }

        // Create MOS
        this->_nmos = this->addCircuit("mos", &nmosArguments);
        this->_pmos = this->addCircuit("mos", &pmosArguments);
    } 

    void NOR::createInstances() 
    {
        // PMOS
        std::vector<std::string_view> net{};
        for (int i = 0; i < this->_inputSize - 1; ++i)
            net.emplace_back( stringFactory->get("net%d", i) );

        for (int i = 0; i < this->_inputSize; ++i)
        {
            Instance* pmos{ this->addInstance(stringFactory->get("nor_pmos%d", i), this->_nmos)};

            if (i == 0)
                this->connectWith(pmos, {"Z", stringFactory->get("A%d", i), net[0], "vdd"});
            else if (i == this->_inputSize - 1)
                this->connectWith(pmos, {net.back(), stringFactory->get("A%d", i), "vdd", "vdd"});
            else
                this->connectWith(pmos, {net[i - 1], stringFactory->get("A%d", i), net[i], "vdd"});
        }
        
        // NMOS
        for (int i = 0; i < this->_inputSize; ++i)
        {
            Instance* nmos{this->addInstance(stringFactory->get("nor_nmos%d", i), this->_nmos)};
            this->connectWith(nmos, {"Z", stringFactory->get("A%d", i), "gnd", "gnd"});
        }
    }
}