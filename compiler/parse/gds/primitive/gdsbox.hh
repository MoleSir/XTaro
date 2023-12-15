#pragma once

#include "../value/gdsvalues.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSBox : public GDSElement
    {
    public:
        GDSBox() noexcept : GDSElement(GDSElementType::BOX) {}
        virtual ~GDSBox() noexcept
        {
            delete this->_layer;
            delete this->_boxtyoe;
        }

    public:
        virtual GDSLayer* layer() noexcept override
        { return this->_layer; }

        virtual GDSBoxType* datatype() noexcept override
        { return this->_boxtyoe; }

    public:
        virtual void setLayer(GDSLayer* layer) noexcept override
        { this->_layer = layer; }

        virtual void setDataType(GDSBoxType* datatype) noexcept override
        { this->_boxtyoe = datatype; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_layer && !this->_boxtyoe && !this->_xy; }

    private:
        GDSLayer* _layer = nullptr;
        GDSBoxType* _boxtyoe = nullptr;
    };
}