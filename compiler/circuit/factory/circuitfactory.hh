#pragma once

#include <string>
#include <string_view>
#include <memory>

namespace xtaro::circuit
{

    class Circuit;
    class CircuitArguments;

    class CircuitFactory
    {
    private:
        CircuitFactory();
        ~CircuitFactory() noexcept;
        CircuitFactory(const CircuitFactory&) = delete;
        CircuitFactory(CircuitFactory&&) noexcept = delete;
        CircuitFactory& operator = (const CircuitFactory&) = delete;
        CircuitFactory& operator = (CircuitFactory&&) noexcept = delete;

    public:
        static CircuitFactory* instance();

    public:
        void reset() noexcept;

        Circuit* create(const std::string_view& circuitType,
                        CircuitArguments* arguments,
                        const std::string_view& circuitName = "");

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };

    extern CircuitFactory* circuitFactory;

}