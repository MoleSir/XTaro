#pragma once

#include "../value/gdsvalues.hh"

namespace xtaro::parser
{
    enum class GDSElementType
    {
        BOUNDARY,
        PATH,
        SREF,
        AREF,
        TEXT,
        NODE,
        BOX,
    };

    class GDSElement
    {
    public:
        GDSElement(GDSElementType type) noexcept:
            _type{type}, _flags{nullptr}, _plex{nullptr}, _xy{nullptr} {}

        virtual ~GDSElement() noexcept
        {
            delete this->_flags;
            delete this->_plex;
            delete this->_xy;
        }

    public:
        GDSElementType type() noexcept
        { return this->_type; }

        GDSElementFlags* flags() noexcept
        { return this->_flags; }

        GDSPlex* plex() noexcept
        { return this->_plex; }

        GDSXY* xy() noexcept
        { return this->_xy; }

    public:
        void setElementFlags(GDSElementFlags* flags) noexcept
        { this->_flags = flags; }

        void setPlex(GDSPlex* plex) noexcept
        { this->_plex = plex; }

        void setXY(GDSXY* xy) noexcept
        { this->_xy = xy; }

    public:
        virtual GDSLayer* layer() noexcept { return nullptr; }
        virtual GDSDataType* datatype() noexcept { return nullptr; }
        virtual GDSPathType* pathtype() noexcept { return nullptr; }
        virtual GDSWidth* width() noexcept { return nullptr; }
        virtual GDSStructureName* structureName() noexcept { return nullptr; }
        virtual GDSStructureTrans* structureTrans() noexcept { return nullptr; }
        virtual GDSColumnRow* columnRow() noexcept { return nullptr; }
        virtual GDSTextType* texttype() noexcept { return nullptr; }
        virtual GDSPresentation* presentation() noexcept { return nullptr; }
        virtual GDSString* string() noexcept { return nullptr; }
        virtual GDSNodeType* nodetype() noexcept { return nullptr; }
        virtual GDSBoxType* boxtype() noexcept { return nullptr; }

    public:
        virtual void setLayer(GDSLayer*) noexcept {}
        virtual void setDataType(GDSDataType*) noexcept {}
        virtual void setPathType(GDSPathType*) noexcept {}
        virtual void setWidth(GDSWidth*) noexcept {}
        virtual void setStructureName(GDSStructureName*) noexcept {}
        virtual void setStructTrans(GDSStructureTrans*) noexcept {}
        virtual void setColumnRow(GDSColumnRow*) noexcept {}
        virtual void setTextType(GDSTextType*) noexcept {}
        virtual void setPresentation(GDSPresentation*) noexcept {}
        virtual void setString(GDSString*) noexcept {}
        virtual void setNodeType(GDSNodeType*) noexcept {}
        virtual void setBoxType(GDSBoxType*) noexcept {}

        virtual bool checkAttributes() const noexcept = 0;

    protected:
        GDSElementType _type;
        GDSElementFlags* _flags;
        GDSPlex* _plex;
        GDSXY* _xy;
    };
}