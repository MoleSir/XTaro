#pragma once

#include <cstdint>
#include <vector>

namespace xtaro::parser
{

    struct GDSCoordinate
    {
        GDSCoordinate(std::int32_t x = 0, std::int32_t y = 0) : x{x}, y{y} {}
        std::int32_t x;
        std::int32_t y;
    };

    class GDSXY
    {
    public:
        GDSXY(int coordinatesSize) : _coordinates{coordinatesSize} {}
        ~GDSXY() = default;

        // Getter
        size_t getSizes() const noexcept
        { return this->_coordinates.size(); }

        const std::vector<GDSCoordinate>& getCoordinates() const noexcept 
        { return this->_coordinates; }

        // Setter
        void addCoordinate(std::int32_t x, std::int32_t y) 
        { this->_coordinates.emplace_back(x, y); }

        void addCoordinate(const GDSCoordinate& coordinate)
        { this->_coordinates.emplace_back(coordinate); }

        // Function
        void clear() 
        { this->_coordinates.clear(); }

    private:
        std::vector<GDSCoordinate> _coordinates;
    };

}