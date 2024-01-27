#include "decoder.hh"

#include <module/and.hh>
#include <module/inv.hh>

#include <config/tech.hh>
#include <util/util.hh>
#include <debug/debug.hh>

namespace xtaro::circuit
{

    const std::array<std::vector<int>, Decoder::COMPONENT_SIZE> 
    Decoder::subDecodersInputSizes
    {
        std::vector<int>{2, 3}, 
        std::vector<int>{3, 3}, 
        std::vector<int>{3, 4}, 
        std::vector<int>{4, 4},
        std::vector<int>{3, 3, 3},
        std::vector<int>{3, 3, 4},
        std::vector<int>{3, 4, 4},
        std::vector<int>{4, 4, 4},
    };

    std::string DecoderArguments::toString() const
    {
        return util::format("is%d", this->inputSize);
    }

    Decoder::Decoder(String name, DecoderArguments* arguments) :
        Circuit{name, DeviceType::SUBCKT},
        _inputSize{arguments->inputSize},
        _outputSize{ util::power(2, this->_inputSize) },
        _and{nullptr},
        _inv{nullptr}
    {
        // check input size range!!!
        if (this->_inputSize <= 1 || this->_inputSize > Decoder::MAX_INPUT_SIZE)
        {
            std::string errorMsg {util::format("Decoder's input size '%d' is not a vaild", this->_inputSize)};
            debug->errorWithException("Create Decoder", errorMsg);
        }

        debug->debug("Create a 'Decoder' circuit: '%s'", this->_name.cstr());
        this->createNetlist();
    }

    void Decoder::createPorts()
    {
        // Input Ports
        for (int i = 0; i < this->_inputSize; ++i)
            this->addPort(util::format("A%d", i), PortType::INPUT);
        
        // Output Ports
        int outputSize{ util::power(2, this->_inputSize) };
        for (int i = 0; i < outputSize; ++i)
            this->addPort(util::format("Y%d", i), PortType::OUTPUT);

        // vdd & gnd
        this->addPort("vdd", PortType::INOUT);
        this->addPort("gnd", PortType::INOUT);
    }

    void Decoder::createCircuits()
    {   
        // Create inv
        INVArguments invArguments{DriveCapability::NORMAL};
        this->_inv = this->addCircuit("inv", &invArguments);

        // Get AND Gate input size
        int andGateInputSize{-1};
        if (this->_inputSize <= Decoder::MAX_SIMPLE_INPUT_SIZE)
            andGateInputSize = this->_inputSize;
        else
            andGateInputSize = this->subDecodersInputSize().size();

        // Create and
        ANDArguments andArguments{DriveCapability::NORMAL, andGateInputSize};
        this->_and = this->addCircuit("and", &andArguments);

        // Create Sub Decoders
        if (this->_inputSize > Decoder::MAX_SIMPLE_INPUT_SIZE)
            this->createSubDecoders();
    }

    void Decoder::createInstances() 
    {
        if (this->_inputSize <= Decoder::MAX_SIMPLE_INPUT_SIZE)
            this->createSimpleInstances();
        else
            this->createComponetInstances(); 
    }

    void Decoder::createSubDecoders()
    {
        const std::vector<int>& subDecodersInputSize{this->subDecodersInputSize()};

        for (std::size_t i = 0; i < subDecodersInputSize.size(); ++i)
        {
            DecoderArguments argument {subDecodersInputSize[i]};
            Circuit* decoder{ this->addCircuit(
                "decoder", &argument, util::format("decoder_%d", argument.inputSize)
            ) };
            this->_subDecoders.emplace_back(decoder);
        }
    }

    void Decoder::createSimpleInstances()
    {
        std::vector<String> inputPorts{};
        std::vector<String> inputPortsInt{};
        for (int i = 0; i < this->_inputSize; ++i)
        {
            inputPorts.emplace_back( util::format("A%d", i) );
            inputPortsInt.emplace_back( util::format("A%d_bar", i) );
        }

        // Create INV for input 
        for (int i = 0; i < this->_inputSize; ++i)
        {
            Instance* inv{ this->addInstance(util::format("inv%d", i), this->_inv) };
            this->connectWith(inv, {inputPorts[i], inputPortsInt[i], "vdd", "gnd"});
        }

        // Create AND
        int outputSize{ util::power(2, this->_inputSize) };
        for (int i = 0; i < this->_outputSize; ++i)
        {
            // Well, the literal 'and' is a C++ keyword ...
            Instance* andGate{ this->addInstance(
                util::format("and%d", i), this->_and
            ) };
            
            // Create nets list to connect with 'andInstance'
            std::vector<String> nets{};
            for (int j = 0; j < this->_inputSize; ++j)
            {
                // 'i' is the AND gate's index. Each AND gate's inputs are [A0/A0_int, A1/A1_int ... An/An_int]
                // There are '_inputSize' inputs, and 'j' is the input port' index.
                // No.'j' bit in 'i' decides the port for Aj is inverted or not.
                // For example, i == 000, the inputs are [A0_int, A1_int, A2_int].
                //              i == 010, the inputs are [A0_int, A1,     A2_int].
                // ...
                bool bitOne{ ((i >> j) & 0x1) != 0 };
                nets.emplace_back( bitOne ? inputPorts[j] : inputPortsInt[j] );
            }

            nets.emplace_back( util::format("Y%d", i) );
            nets.emplace_back("vdd");
            nets.emplace_back("gnd");

            this->connectWith(andGate, nets);
        }
    }

    void Decoder::createComponetInstances()
    {
        // Create sub decoder
        const std::vector<int>& subDecodersInputSize{this->subDecodersInputSize()};
        int inputPortIndex{0};

        // Loop for each sub decoder
        for (std::size_t decoderIdx = 0; decoderIdx < subDecodersInputSize.size(); ++decoderIdx)
        {
            // Get sub decoder's input size
            int subDecoderInputSize{subDecodersInputSize[decoderIdx]};
            
            // Create decoder instance
            Instance* decoder{ this->addInstance(
                util::format("decoder%d", decoderIdx), this->_subDecoders[decoderIdx]
            ) };
            
            // Connect nets
            // Create the nets' name
            std::vector<String> nets{};
            // Input ports
            for (int i = 0; i < subDecoderInputSize; ++i)
                nets.emplace_back( util::format("A%d", inputPortIndex++) );
            // Output ports
            for (int outputIdx = 0; outputIdx < util::power(2, subDecoderInputSize); ++outputIdx)
                nets.emplace_back( util::format("Y_%d_%d", decoderIdx, outputIdx) );
            // vdd & gnd
            nets.emplace_back("vdd");
            nets.emplace_back("gnd");
            
            this->connectWith(decoder, nets);
        }

        // Loop for each Output AND gate
        for (int andGateIdx = 0; andGateIdx < this->_outputSize; ++andGateIdx)
        {
            // Create AND gate instance
            Instance* andGate{ this->addInstance(
                util::format("and%d", andGateIdx), this->_and
            ) };
        
            // Connect nets...
            std::vector<String> nets{};
            
            // For each decoder, get an output line as AND gate's input
            // emmm... this algo is hard to explain, so TODO!!!
            for (std::size_t decoderIdx = 0; decoderIdx < subDecodersInputSize.size(); ++decoderIdx)
            {
                int prefixSum{0};
                for (std::size_t idx = 0; idx < decoderIdx; ++idx)
                    prefixSum += subDecodersInputSize[idx];
                
                int mask{1};
                for (int i = 1; i < subDecodersInputSize[decoderIdx]; ++i)
                    mask = (mask << 1) + 1;

                int decoderOutputIdx{ (andGateIdx >> prefixSum) & mask };
                nets.emplace_back(util::format("Y_%d_%d", decoderIdx, decoderOutputIdx));
            }

            // Output & vdd & gnd
            nets.emplace_back(util::format("Y%d", andGateIdx));
            nets.emplace_back("vdd");
            nets.emplace_back("gnd");

            this->connectWith(andGate, nets);
        }
    }
}