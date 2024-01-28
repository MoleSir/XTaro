#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

#include <vector>
#include <array>

namespace xtaro::circuit
{

    /*
        - Construction: (int inputSize);
    */
    struct DecoderArguments : public CircuitArguments
    {
        DecoderArguments(int is) : inputSize{is} {}

        virtual ~DecoderArguments() noexcept override {}

        virtual std::string toString() const override;

        int inputSize;
    };

    /*
        - Ports sequency: A0 A1 ... An Y0 Y1 ... Y2^n-1 vdd gnd
    */
    class Decoder : public Circuit
    {
    public:
        enum 
        {
            MAX_SIMPLE_INPUT_SIZE = 4,
            MAX_INPUT_SIZE = 12,
            COMPONENT_SIZE = MAX_INPUT_SIZE - MAX_SIMPLE_INPUT_SIZE,
        };

        static const std::array<std::vector<int>, COMPONENT_SIZE> subDecodersInputSizes; 

    public:
        Decoder(const std::string_view& name, DecoderArguments* arguments);
        virtual ~Decoder() noexcept override = default;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        int subDecodersIndex() const noexcept
        { return this->_inputSize - 1 - MAX_SIMPLE_INPUT_SIZE; }

        const std::vector<int>& subDecodersInputSize() const noexcept
        { return Decoder::subDecodersInputSizes[this->subDecodersIndex()]; }

    private:
        void createSubDecoders();

        void createSimpleInstances();
        void createComponetInstances();

    private:
        int _inputSize;
        int _outputSize;

        Circuit* _and;
        Circuit* _inv;

        std::vector<Circuit*> _subDecoders;
    };

}