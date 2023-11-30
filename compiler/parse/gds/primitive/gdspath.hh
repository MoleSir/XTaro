#pragma once

#include "../value/gdsvalue.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSPath : public GDSElement
    {
    public:
        GDSPath() noexcept : GDSElement(GDSElementType::PATH) {}
        virtual ~GDSPath() noexcept
        {
            delete this->_layer;
            delete this->_datatype;
            delete this->_pathtype;
            delete this->_datatype;
        }

    public:
        virtual GDSLayer* layer() noexcept override
        { return this->_layer; }

        virtual GDSDataType* datatype() noexcept override
        { return this->_datatype; }

        virtual GDSPathType* pathtype() noexcept override
        { return this->_pathtype; }

        virtual GDSWidth* width() noexcept override
        { return this->_width; }

    public:
        virtual void setLayer(GDSLayer* layer) noexcept override
        { this->_layer = layer; }

        virtual void setDataType(GDSDataType* datatype) noexcept override
        { this->_datatype = datatype; }

        virtual void setPathType(GDSPathType* pathtype) noexcept override
        { this->_pathtype = pathtype; }

        virtual void setWidth(GDSWidth* width) noexcept override
        { this->_width = width; } 

        virtual bool checkAttributes() const noexcept override
        { return !this->_layer && !this->_datatype && !this->_xy; }

    private:
        GDSLayer* _layer = nullptr;
        GDSDataType* _datatype = nullptr;
        GDSPathType* _pathtype = nullptr;
        GDSWidth* _width = nullptr;
    };
}