#include "circuitfactory.hh"
#include <stringpool/string.hh>
#include <allocator/allocator.hh>
#include <config/tech.hh>

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

#include <util/format.hh>

namespace xtaro::circuit
{
    #define INT(v) static_cast<int>(v)

    const std::array<const char*, CIRCUIT_TYPE_SIZE> CircuitFactory::modulesName 
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

    CircuitFactory* factory{CircuitFactory::instance()};

    Circuit* CircuitFactory::create(CircuitType circuitType, 
                                    CircuitArguments* arguments, 
                                    String circuitName)
    {
        // Is target circuit exits
        std::string argsList{ arguments == nullptr ? "" : arguments->toString() };
        Circuit* circuit{this->findCircuit(circuitType, argsList)};
        if (circuit != nullptr)
            return circuit;

        // Get circuit's name
        // TODO:
        // Well, if a module needs to the same module as its reference circuit.
        // Such as Decoder, the default name will be not right...
        if (circuitName.empty())
            circuitName = this->getDefaultCircuitName(circuitType);

        // Create a new circuit'
        circuit = this->createNewCircuit(circuitType, arguments, std::move(circuitName));

        // Collect circuit
        this->collectCircuit(circuitType, argsList, circuit);

        return circuit;
    }

    CircuitFactory* CircuitFactory::instance()
    {
        static CircuitFactory _factory;
        return &_factory;
    }

    Circuit* CircuitFactory::findCircuit(CircuitType circuitType, 
                                         const std::string& argsList) const
    {   
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        auto iter{ circuits.find(argsList) };
        if (iter != circuits.end())
            return iter->second;
        return nullptr;
    }

    String CircuitFactory::getDefaultCircuitName(CircuitType circuitType) const
    {
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        if (circuits.empty())
            return CircuitFactory::modulesName[INT(circuitType)];
        return util::format("%s_%d", 
                            CircuitFactory::modulesName[INT(circuitType)], 
                            circuits.size());
    }

    Circuit* CircuitFactory::createNewCircuit(
             CircuitType circuitType, 
             CircuitArguments* arguments,
             String circuitName) const
    {
        #define ALLOCATE_MODULE(ModuleClass)\
        Allocator::alloc<ModuleClass>(std::move(circuitName), dynamic_cast<ModuleClass ## Arguments*>(arguments));

        switch (circuitType)
        {
        case CircuitType::BITCELL: return ALLOCATE_MODULE(Bitcell);
        case CircuitType::DFF: return ALLOCATE_MODULE(DFF);
        case CircuitType::PRECHARGE: return ALLOCATE_MODULE(Precharge);
        case CircuitType::SENSE_AMPLIFIER: return ALLOCATE_MODULE(SenseAmplifier);
        case CircuitType::TRISTATE_GATE: return ALLOCATE_MODULE(TristateGate);
        case CircuitType::WRITE_DRIVER: return ALLOCATE_MODULE(WriteDriver);
        case CircuitType::BITCELL_ARRAY: return ALLOCATE_MODULE(BitcellArray);
        case CircuitType::MOS: return ALLOCATE_MODULE(MOS);
        case CircuitType::INV: return ALLOCATE_MODULE(INV);
        case CircuitType::NAND: return ALLOCATE_MODULE(NAND);
        case CircuitType::AND: return ALLOCATE_MODULE(AND);
        case CircuitType::NOR: return ALLOCATE_MODULE(NOR);
        case CircuitType::OR: return ALLOCATE_MODULE(OR);
        case CircuitType::DECODER: return ALLOCATE_MODULE(Decoder);
        case CircuitType::ROW_DECODER: return ALLOCATE_MODULE(RowDecoder);
        case CircuitType::MUX: return ALLOCATE_MODULE(Mux);
        case CircuitType::COLUMN_MUX: return ALLOCATE_MODULE(ColumnMux);
        case CircuitType::FANOUT_BUFFER: return ALLOCATE_MODULE(FanoutBuffer);
        case CircuitType::SENSE_AMPLIFIER_ARRAY: return ALLOCATE_MODULE(SenseAmplifierArray);
        case CircuitType::WRITE_DRIVER_ARRAY: return ALLOCATE_MODULE(WriteDriverArray);
        case CircuitType::PRECHARGE_ARRAY: return ALLOCATE_MODULE(PrechargeArray);
        case CircuitType::REPLICA_BANK: return ALLOCATE_MODULE(ReplicaBank);
        case CircuitType::BANK: return ALLOCATE_MODULE(Bank);
        case CircuitType::CONTROL_LOGIC: return ALLOCATE_MODULE(ControlLogic);
        case CircuitType::SRAM: return ALLOCATE_MODULE(SRAM);
        }

        #undef ALLOCATE_MODULE

        return nullptr;
    }

    void CircuitFactory::collectCircuit(CircuitType circuitType, 
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