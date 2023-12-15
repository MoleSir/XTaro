#pragma once

#include "../value/gdsvalues.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSBoundary : public GDSElement
    {
    public:
        GDSBoundary() noexcept : GDSElement(GDSElementType::BOUNDARY) {}
        virtual ~GDSBoundary() noexcept
        {
            delete this->_layer;
            delete this->_datatype;
        }

    public:
        virtual GDSLayer* layer() noexcept override
        { return this->_layer; }

        virtual GDSDataType* datatype() noexcept override
        { return this->_datatype; }

    public:
        virtual void setLayer(GDSLayer* layer) noexcept override
        { this->_layer = layer; }

        virtual void setDataType(GDSDataType* datatype) noexcept override
        { this->_datatype = datatype; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_layer && !this->_datatype && !this->_xy; }

    private:
        GDSLayer* _layer = nullptr;
        GDSDataType* _datatype = nullptr;
    };
}