#pragma once

#include "../value/gdsvalue.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSStructureReference : public GDSElement
    {
    public:
        GDSStructureReference() noexcept : GDSElement(GDSElementType::SREF) {}
        virtual ~GDSStructureReference() noexcept
        {
            delete this->_sname;
            delete this->_strans;
        }

    public:
        virtual GDSStructureName* structName() noexcept
        { return this->_sname; }

        virtual GDSStructureTrans* structureTrans() noexcept
        { return this->_strans; }

    public:
        virtual void setStructureName(GDSStructureName* sname) noexcept override
        { this->_sname = sname; }

        virtual void setStructTrans(GDSStructureTrans* strans) noexcept override
        { this->_strans = strans; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_sname && !this->_xy; }

    private:
        GDSStructureName* _sname = nullptr;
        GDSStructureTrans* _strans = nullptr;
    };
}