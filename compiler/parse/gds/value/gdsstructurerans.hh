#pragma once

#include <cstdint>

namespace xtaro::parser
{   

    class GDSStructureTrans
    {
    public:
        union FlagsValue
        {
            FlagsValue(std::uint16_t v) : value{v} {}
            std::uint16_t value;
            struct
            {
                unsigned mirrorX : 1;
                unsigned unused : 13;
                unsigned useAbsMagnification : 1;
                unsigned useAbsRotation : 1;
            };
        };

    public:
        GDSStructureTrans() : _value{0}, _magnification{0.0}, _angle{0.0} {}

        // Check
        bool isMirrorX() const noexcept
        { return this->_value.mirrorX; }

        bool isUseAbsMagnification() const noexcept
        { return this->_value.useAbsMagnification; }

        bool isUseAbsRotation() const noexcept
        { return this->_value.useAbsRotation; }

        // Getter
        double magnification() const noexcept
        { return (this->isUseAbsMagnification()) ? this->_magnification : 0.0; }

        double angle() const noexcept
        { return (this->isUseAbsRotation()) ? this->_angle : 0.0; }

        // Setter
        void setIsMirrorX(bool enabled) noexcept
        { this->_value.mirrorX = enabled; }

        void setIsUseAbsMagnification(bool enabled) noexcept
        { this->_value.useAbsMagnification = enabled; }

        void setIsUseAbsRotation(bool enabled) noexcept
        { this->_value.useAbsRotation = enabled; }

        void setMagnification(double magnification) noexcept
        {
            if (this->isUseAbsMagnification())
                this->setIsUseAbsMagnification(true);
            this->_magnification = magnification;
        }

        void setAngle(double angle) noexcept
        {
            if (this->isUseAbsRotation())
                this->setIsUseAbsRotation(true);
            this->_angle = angle;
        }

    private:
        FlagsValue _value;
        double _magnification;
        double _angle;
    };

}
