#include "circuitfactory.hh"
#include <stringpool/string.hh>
#include <allocator/allocator.hh>
#include <tech/tech.hh>
#include <log/logger.hh>

#include <base/circuit.hh>
#include <module/bitcell.hh>
#include <module/dff.hh>
#include <module/precharge.hh>
#include <module/senseamp.hh>
#include <module/trigate.hh>
#include <module/writedriver.hh>
#include <module/bitcellarray.hh>
#include <module/mos.hh>
#include <module/inv.hh>
#include <module/nand.hh>
#include <module/and.hh>
#include <module/nor.hh>
#include <module/or.hh>
#include <module/decoder.hh>
#include <module/rowdecoder.hh>
#include <module/mux.hh>
#include <module/columnmux.hh>
#include <module/fanoutbuff.hh>
#include <module/senseamparr.hh>
#include <module/writedriverarr.hh>
#include <module/prechargearr.hh>
#include <module/replicabank.hh>
#include <module/bank.hh>
#include <module/controllogic.hh>
#include <module/sram.hh>

#include <util/util.hh>

namespace xtaro::circuit
{
    #define INT(v) static_cast<int>(v)

    const std::array<const char*, MODULESIZE> CircuitFactory::modulesName 
    {   
        "bitcell", 
        "dff",
        "precharge",
        "sense_amp",
        "tri_gate",
        "write_driver",
        "bitcell_array",
        "", // MOS name is special!!!
        "inv",
        "nand",
        "and",
        "nor",
        "or",
        "decoder",
        "row_decoder",
        "mux",
        "column_mux",
        "fanout_buffer",
        "sense_amp_arr",
        "write_driver_arr",
        "precharge_array",
        "replica_bank",
        "bank",
        "control_logic",
        "sram",
    };

#define NDEBUG

    CircuitFactory* factory{CircuitFactory::instance()};

    Circuit* CircuitFactory::create(ModuleType circuitType, 
                                    CircuitArguments* arguments, 
                                    String circuitName)
    {
        // Is target circuit exits
#ifndef NDEBUG
        std::cout << 1 << std::endl;
#endif
        std::string argsList{ arguments == nullptr ? "" : arguments->toString() };
        Circuit* circuit{this->findCircuit(circuitType, argsList)};
        if (circuit != nullptr)
            return circuit;

        // Get circuit's name
#ifndef NDEBUG
        std::cout << 2 << std::endl;
#endif
        // TODO:
        // Well, if a module needs to the same module as its reference circuit.
        // Such as Decoder, the default name will be not right...
        if (circuitName.empty())
            circuitName = this->getDefaultCircuitName(circuitType);

        // Create a new circuit'
#ifndef NDEBUG
        std::cout << 3 << std::endl;
#endif
        circuit = this->createNewCircuit(circuitType, arguments, std::move(circuitName));

        // Collect circuit
#ifndef NDEBUG
        std::cout << 4 << std::endl;
#endif
        this->collectCircuit(circuitType, argsList, circuit);

#ifndef NDEBUG
        std::cout << 5 << std::endl;
#endif
        return circuit;
    }

    CircuitFactory* CircuitFactory::instance()
    {
        static CircuitFactory _factory;
        return &_factory;
    }

    Circuit* CircuitFactory::findCircuit(ModuleType circuitType, 
                                         const std::string& argsList) const
    {   
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        auto iter{ circuits.find(argsList) };
        if (iter != circuits.end())
            return iter->second;
        return nullptr;
    }

    String CircuitFactory::getDefaultCircuitName(ModuleType circuitType) const
    {
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        if (circuits.empty())
            return CircuitFactory::modulesName[INT(circuitType)];
        return util::format("%s_%d", 
                            CircuitFactory::modulesName[INT(circuitType)], 
                            circuits.size());
    }

    Circuit* CircuitFactory::createNewCircuit(
             ModuleType circuitType, 
             CircuitArguments* arguments,
             String circuitName) const
    {
#define ALLOCATE_MODULE(ModuleClass)\
Allocator::alloc<ModuleClass>(std::move(circuitName), dynamic_cast<ModuleClass ## Arguments*>(arguments));

#ifndef NDEBUG
        std::cout << "create new circuit" << std::endl;
#endif
        switch (circuitType)
        {
        case ModuleType::BITCELL: return ALLOCATE_MODULE(Bitcell);
        case ModuleType::DFF: return ALLOCATE_MODULE(DFF);
        case ModuleType::PRECHARGE: return ALLOCATE_MODULE(Precharge);
        case ModuleType::SENSE_AMPLIFIER: return ALLOCATE_MODULE(SenseAmplifier);
        case ModuleType::TRISTATE_GATE: return ALLOCATE_MODULE(TristateGate);
        case ModuleType::WRITE_DRIVER: return ALLOCATE_MODULE(WriteDriver);
        case ModuleType::BITCELL_ARRAY: return ALLOCATE_MODULE(BitcellArray);
        case ModuleType::MOS: return ALLOCATE_MODULE(MOS);
        case ModuleType::INV: return ALLOCATE_MODULE(INV);
        case ModuleType::NAND: return ALLOCATE_MODULE(NAND);
        case ModuleType::AND: return ALLOCATE_MODULE(AND);
        case ModuleType::NOR: return ALLOCATE_MODULE(NOR);
        case ModuleType::OR: return ALLOCATE_MODULE(OR);
        case ModuleType::DECODER: return ALLOCATE_MODULE(Decoder);
        case ModuleType::ROW_DECODER: return ALLOCATE_MODULE(RowDecoder);
        case ModuleType::MUX: return ALLOCATE_MODULE(Mux);
        case ModuleType::COLUMN_MUX: return ALLOCATE_MODULE(ColumnMux);
        case ModuleType::FANOUT_BUFFER: return ALLOCATE_MODULE(FanoutBuffer);
        case ModuleType::SENSE_AMPLIFIER_ARRAY: return ALLOCATE_MODULE(SenseAmplifierArray);
        case ModuleType::WRITE_DRIVER_ARRAY: return ALLOCATE_MODULE(WriteDriverArray);
        case ModuleType::PRECHARGE_ARRAY: return ALLOCATE_MODULE(PrechargeArray);
        case ModuleType::REPLICA_BANK: return ALLOCATE_MODULE(ReplicaBank);
        case ModuleType::BANK: return ALLOCATE_MODULE(Bank);
        case ModuleType::CONTROL_LOGIC: return ALLOCATE_MODULE(ControlLogic);
        case ModuleType::SRAM: return ALLOCATE_MODULE(SRAM);
        }
        return nullptr;
    }

    void CircuitFactory::collectCircuit(ModuleType circuitType, 
                                        std::string argsList,
                                        Circuit* circuit)
    {
        auto& circuits{ this->_circuits[INT(circuitType)] };
        circuits.emplace( std::move(argsList), circuit );
    }

    CircuitFactory::~CircuitFactory() noexcept
    {
        for (auto& circuits : this->_circuits)
        {
            for (auto& kv : circuits)
            {
                Allocator::free(kv.second);
                kv.second = nullptr;
            }
        }
    }
}