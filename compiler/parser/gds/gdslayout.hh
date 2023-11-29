#pragma once

#include "./value/gdsvalue.hh"
#include "./primitive/gdsstructrue.hh"

#include <list>

namespace xtaro::parser
{

    class GDSLayout
    {
    private:
        GDSHeader* _header;
        GDSTimestamp* _timestamp;
        GDSRefenceLibs* _reflibs;
        GDSFonts* _fonts;
        GDSAttrtable* _attrtable;
        GDSGenerations* _generations;
        GDSFormat* _format;

        std::list<GDSStructure*> _structures; 
    };

}