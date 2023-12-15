#pragma once

#include "gdsvalue.hh"
#include "gdselementflags.hh"
#include "gdstimestamp.hh"
#include "gdsunits.hh"
#include "gdsstructurerans.hh"
#include "gdsreflibs.hh"
#include "gdsfonts.hh"
#include "gdsxy.hh"
#include "gdscolrow.hh"

#include <cstdint>
#include <string>

namespace xtaro::parser
{

    using GDSHeader = GDSInt16;
    using GDSGenerations = GDSInt16;
    using GDSPlex = GDSInt32;
    using GDSLayer = GDSInt16;
    using GDSDataType = GDSInt16;
    using GDSWidth = GDSInt16;
    using GDSTextType = GDSInt16;
    using GDSNodeType = GDSInt16;
    using GDSBoxType = GDSInt16;
    using GDSStructureName = GDSString;
    using GDSPresentation = GDSInt16;
    using GDSFormat = GDSInt16;
    using GDSAttrtable = GDSString;

    enum class GDSPathType
    {
        SQUARE_END = 0,
        ROUND_END = 1,
        EXTEND_SQUARE_END = 2,
    };

    
}