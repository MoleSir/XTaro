#include "factoryimpl.hh"
#include <stringpool/string.hh>
#include <allocator/allocator.hh>
#include <config/tech.hh>
#include <debug/debug.hh>

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

#include <string_view>

namespace xtaro::circuit
{
    std::unordered_map<std::string_view, CircuitType> 
    CircuitFactory::Impl::circuitTypes
    {
        {std::string_view{"bitcell"}, CircuitType::BITCELL},
        {std::string_view{"dff"}, CircuitType::DFF},
        {std::string_view{"precharge"}, CircuitType::PRECHARGE},
        {std::string_view{"sense_amp"}, CircuitType::SENSE_AMPLIFIER},
        {std::string_view{"tristate_gate"}, CircuitType::TRISTATE_GATE},
        {std::string_view{"write_driver"}, CircuitType::WRITE_DRIVER},
        {std::string_view{"bitcell_array"}, CircuitType::BITCELL_ARRAY},
        {std::string_view{"mos"}, CircuitType::MOS},
        {std::string_view{"inv"}, CircuitType::INV},
        {std::string_view{"nand"}, CircuitType::NAND},
        {std::string_view{"and"}, CircuitType::AND},
        {std::string_view{"nor"}, CircuitType::NOR},
        {std::string_view{"or"}, CircuitType::OR},
        {std::string_view{"decoder"}, CircuitType::DECODER},
        {std::string_view{"row_decoder"}, CircuitType::ROW_DECODER},
        {std::string_view{"mux"}, CircuitType::MUX},
        {std::string_view{"column_mux"}, CircuitType::COLUMN_MUX},
        {std::string_view{"fanout_buffer"}, CircuitType::FANOUT_BUFFER},
        {std::string_view{"sense_amp_array"}, CircuitType::SENSE_AMPLIFIER_ARRAY},
        {std::string_view{"write_driver_array"}, CircuitType::WRITE_DRIVER_ARRAY},
        {std::string_view{"precharge_array"}, CircuitType::PRECHARGE_ARRAY},
        {std::string_view{"replica_bank"}, CircuitType::REPLICA_BANK},
        {std::string_view{"bank"}, CircuitType::BANK},
        {std::string_view{"control_logic"}, CircuitType::CONTROL_LOGIC},
        {std::string_view{"sram"}, CircuitType::SRAM},
    };

    #define INT(v) static_cast<int>(v)

    Circuit* CircuitFactory::Impl::create(
        const std::string_view& circuitTypeName, 
        CircuitArguments* arguments, 
        String circuitName)
    {
        // Get circuit type
        CircuitType circuitType {this->getCircuitType(circuitTypeName)};
        if (CircuitType::SIZE == circuitType)
        {
            debug->errorWithException(
                "Create Circuit", util::format("Unkonw circuit '%s'", circuitTypeName.data())
            );
        }

        // Is target circuit exits
        std::string argsList{ arguments == nullptr ? "" : arguments->toString() };
        Circuit* circuit {this->findCircuit(circuitType, argsList)};
        if (circuit != nullptr)
            return circuit;

        // Get circuit's name
        // TODO:
        // Well, if a module needs to the same module as its reference circuit.
        // Such as Decoder, the default name will be not right...
        if (circuitName.empty())
            circuitName = this->getDefaultCircuitName(circuitTypeName, circuitType);

        // Create a new circuit'
        circuit = this->createNewCircuit(circuitType, arguments, std::move(circuitName));

        // Collect circuit
        this->collectCircuit(circuitType, argsList, circuit);

        return circuit;
    }

    void CircuitFactory::Impl::reset() noexcept
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

    CircuitType CircuitFactory::Impl::getCircuitType(const std::string_view& circuitType)
    {
        auto iter {Impl::circuitTypes.find(std::string_view(circuitType))};
        if (iter == Impl::circuitTypes.end())
            return CircuitType::SIZE;
        return iter->second;
    }

    Circuit* CircuitFactory::Impl::findCircuit(CircuitType circuitType, 
                                         const std::string& argsList) const
    {   
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        auto iter { circuits.find(argsList) };
        if (iter != circuits.end())
            return iter->second;

        return nullptr;
    }

    String CircuitFactory::Impl::getDefaultCircuitName(
        const std::string_view& circuitTypeName,
        CircuitType circuitType) const
    {
        const auto& circuits{ this->_circuits[INT(circuitType)] };

        if (circuits.empty())
            return String{circuitTypeName.data()};
        return String{ util::format(
                            "%s_%d", 
                            circuitTypeName.data(), 
                            circuits.size()) };
    }

    Circuit* CircuitFactory::Impl::createNewCircuit(
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

    void CircuitFactory::Impl::collectCircuit(CircuitType circuitType, 
                                        std::string argsList,
                                        Circuit* circuit)
    {
        auto& circuits{ this->_circuits[INT(circuitType)] };
        circuits.emplace( std::move(argsList), circuit );
    }

    CircuitFactory::Impl::~Impl() noexcept
    {
        this->reset();
    }
}