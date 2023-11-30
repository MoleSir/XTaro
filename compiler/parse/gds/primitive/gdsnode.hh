#pragma once

#include "../value/gdsvalue.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSNode : public GDSElement
    {
    public:
        GDSNode() noexcept : GDSElement(GDSElementType::NODE) {}
        virtual ~GDSNode() noexcept
        {
            delete this->_layer;
            delete this->_nodetype;
        }

    public:
        virtual GDSLayer* layer() noexcept override
        { return this->_layer; }

        virtual GDSNodeType* nodetype() noexcept override
        { return this->_nodetype; }

    public:
        virtual void setLayer(GDSLayer* layer) noexcept override
        { this->_layer = layer; }

        virtual void setNodeType(GDSNodeType* nodetype) noexcept override
        { this->_nodetype = nodetype; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_layer && !this->_nodetype && !this->_xy; }

    private:
        GDSLayer* _layer = nullptr;
        GDSNodeType* _nodetype = nullptr;
    };
}