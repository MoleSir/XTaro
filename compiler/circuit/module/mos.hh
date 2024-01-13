#pragma once

#include <base/circuitenum.hh>
#include <base/circuit.hh>

namespace xtaro::circuit
{

    /*
        - Construction: (MOSType type, double width, double length);
    */
    struct MOSArguments : public CircuitArguments
    {
        MOSArguments(MOSType t, double w, double l) :
            type{t}, width{w}, length{l} {}
            
        virtual ~MOSArguments() noexcept override {}

        virtual std::string toString() const override;

        MOSType type;
        double width;
        double length;
    };

    /*
        - Ports sequency: d/s g s/d b
    */
    class MOS : public Circuit
    {
    public:
        MOS(std::string name, MOSArguments* arguments);
        virtual ~MOS() noexcept override = default;

    public:
        virtual std::string argumentsList() const override;

    private:
        virtual void createPorts() override;
        virtual void createCircuits() override;
        virtual void createInstances() override;

    private:
        // TODO: m=1 w=1.6u l=0.4u pd=4.00u ps=4.00u as=1.60p ad=1.60p
        // other arguments
        MOSType _type;
        double _width;
        double _length;
    };

}