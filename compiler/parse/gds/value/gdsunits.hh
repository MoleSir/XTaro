#pragma once

namespace xtaro::parser 
{

    class GDSUnits
    {
    public:
        GDSUnits(double userUnits = 0.0, double unitMeters = 0.0) :
            _userUnits{userUnits}, _unitMeters{unitMeters} {}

        // Getter
        double userUnits() const noexcept
        { return this->_userUnits; }

        double unitMeters() const noexcept
        { return this->_unitMeters; }

        // Setter
        void setUserUnits(double userUnits) noexcept
        { this->_userUnits = userUnits; }

        void setUnitMeters(double unitMeters) noexcept
        { this->_unitMeters = unitMeters; }
         
    private:
        double _userUnits;
        double _unitMeters;
    };

}
