#pragma once

#include "../value/gdsvalue.hh"
#include "gdselement.hh"

namespace xtaro::parser
{
    class GDSArrayReference : public GDSElement
    {
    public:
        GDSArrayReference() noexcept : GDSElement() {}
        virtual ~GDSArrayReference() noexcept
        {
            delete this->_sname;
            delete this->_strans;
        }

    public:
        virtual GDSStructureName* structName() noexcept
        { return this->_sname; }

        virtual GDSStructureTrans* structureTrans() noexcept
        { return this->_strans; }

        virtual GDSColumnRow* columnRow() noexcept
        { return this->_columnRow; }

    public:
        virtual void setStructureName(GDSStructureName* sname) noexcept override
        { this->_sname = sname; }

        virtual void setStructTrans(GDSStructureTrans* strans) noexcept override
        { this->_strans = strans; }

        virtual void setColumnRow(GDSColumnRow* columnRow) noexcept override
        { this->_columnRow = columnRow; }

        virtual bool checkAttributes() const noexcept override
        { return !this->_sname && !this->_columnRow && !this->_xy; }

    private:
        GDSStructureName* _sname = nullptr;
        GDSStructureTrans* _strans = nullptr;
        GDSColumnRow* _columnRow = nullptr;
    };
}