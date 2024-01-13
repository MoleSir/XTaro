#include "inv.hh"

#include <factory/circuitfactory.hh>
#include <module/mos.hh>
#include <tech/tech.hh>

#include <allocator/allocator.hh>
#include <util/util.hh>

namespace xtaro::circuit
{
    std::string INVArguments::toString() const
    {
        return util::format("dc%d", static_cast<int>(this->driveCapability));
    }

    INV::INV(std::string name, INVArguments* arguments) :
        Circuit{std::move(name), DeviceType::SUBCKT},
        _driveCapability{arguments->driveCapability},
        _nmos{nullptr},
        _pmos{nullptr}
    {
        this->createNetlist();
    }

    void INV::createPorts()
    {
        this->addPort("A", PortType::INPUT);
        this->addPort("Z", PortType::OUTPUT);
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void INV::createCircuits()
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

    void INV::createInstances() 
    {
        // Create instances
        Instance* nmos{ Allocator::alloc<Instance>("inv_nmos", this->_nmos) };
        Instance* pmos{ Allocator::alloc<Instance>("inv_pmos", this->_pmos) };
        this->_instances.push_back(nmos);
        this->_instances.push_back(pmos);

        // Connect
        this->connectWith(pmos, {"Z", "A", "vdd", "vdd"});
        this->connectWith(nmos, {"Z", "A", "gnd", "gnd"});
    }
}