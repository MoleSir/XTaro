#include "nor.hh"

#include <factory/circuitfactory.hh>
#include <module/mos.hh>

#include <allocator/allocator.hh>
#include <util/util.hh>
#include <tech/tech.hh>

namespace xtaro::circuit
{
    std::string NORArguments::toString() const
    {
        return util::format("dc%d_is%d", 
                            static_cast<int>(this->driveCapability),
                            this->inputSize);
    }

    NOR::NOR(String name, NORArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _inputSize{arguments->inputSize},
        _nmos{nullptr},
        _pmos{nullptr}
    {
        this->createNetlist();
    }

    void NOR::createPorts()
    {
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);

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
        this->_nmos = factory->create(ModuleType::MOS, &nmosArguments);
        this->_pmos = factory->create(ModuleType::MOS, &pmosArguments);
        this->_circuits.insert(this->_nmos);
        this->_circuits.insert(this->_pmos);
    } 

    void NOR::createInstances() 
    {
        // PMOS
        std::vector<String> net{};
        for (int i = 0; i < this->_inputSize - 1; ++i)
            net.emplace_back( util::format("net%d", i) );

        for (int i = 0; i < this->_inputSize; ++i)
        {
            Instance* pmos{ this->addInstance(util::format("nor_pmos%d", i), this->_nmos)};

            if (i == 0)
                this->connectWith(pmos, {"Z", util::format("A%d", i), net[0], "vdd"});
            else if (i == this->_inputSize - 1)
                this->connectWith(pmos, {net.back(), util::format("A%d", i), "vdd", "vdd"});
            else
                this->connectWith(pmos, {net[i - 1], util::format("A%d", i), net[i], "vdd"});
        }
        
        // NMOS
        for (int i = 0; i < this->_inputSize; ++i)
        {
            Instance* nmos{this->addInstance(util::format("nor_nmos%d", i), this->_nmos)};
            this->connectWith(nmos, {"Z", util::format("A%d", i), "gnd", "gnd"});
        }
    }
}