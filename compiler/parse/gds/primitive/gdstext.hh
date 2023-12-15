#pragma once

#include "../value/gdsvalues.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSText : public GDSElement
    {
    public:
        GDSText() noexcept: GDSElement(GDSElementType::TEXT) {}
        virtual ~GDSText() noexcept
        {
            delete this->_layer;
            delete this->_texttype;
            delete this->_presentation;
            delete this->_pathtype;
            delete this->_width;
            delete this->_strans;
            delete this->_string;
        }

    public:
        virtual GDSLayer* layer() noexcept override
        { return this->_layer; }

        virtual GDSTextType* texttype() noexcept override
        { return this->_texttype; }

        virtual GDSPresentation* presentation() noexcept override
        { return this->_presentation; }

        virtual GDSPathType* pathtype() noexcept override
        { return this->_pathtype; }

        virtual GDSWidth* width() noexcept override
        { return this->_width; }

        virtual GDSStructureTrans* structureTrans() noexcept override
        { return this->_strans; }

        virtual GDSString* string() noexcept override
        { return this->_string; }

    public:
        virtual void setLayer(GDSLayer* layer) noexcept override
        { this->_layer = layer; }

        virtual void setTextType(GDSTextType* texttype) noexcept override
        { this->_texttype = texttype; }

        virtual void setPresentation(GDSPresentation* presentation) noexcept override
        { this->_presentation = presentation; }

        virtual void setPathType(GDSPathType* pathtype) noexcept override
        { this->_pathtype = pathtype; }

        virtual void setWidth(GDSWidth* width) noexcept override
        { this->_width = width; } 

        virtual void setStructTrans(GDSStructureTrans* strans) noexcept 
        { this->_strans = strans; }

        virtual void setString(GDSString* string) noexcept override
        { this->_string = string; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_layer && !this->_texttype && !this->_xy && !this->_string; }

    private:
        GDSLayer* _layer = nullptr;
        GDSTextType* _texttype = nullptr;
        GDSPresentation* _presentation = nullptr;
        GDSPathType* _pathtype = nullptr;
        GDSWidth* _width = nullptr;
        GDSStructureTrans* _strans = nullptr;
        GDSString* _string = nullptr;
    };
}