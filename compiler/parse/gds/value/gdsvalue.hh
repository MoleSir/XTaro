#pragma once

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

    using GDSHeader = std::int16_t;
    using GDSGenerations = std::int16_t;
    using GDSPlex = std::int32_t;
    using GDSLayer = std::int16_t;
    using GDSDataType = std::int16_t;
    using GDSWidth = std::int16_t;
    using GDSTextType = std::int16_t;
    using GDSNodeType = std::int16_t;
    using GDSBoxType = std::int16_t;
    using GDSString = std::string;
    using GDSStructureName = std::string;
    using GDSPresentation = std::int16_t;
    using GDSFormat = std::int16_t;
    using GDSAttrtable = std::string;

    enum class GDSPathType
    {
        SQUARE_END = 0,
        ROUND_END = 1,
        EXTEND_SQUARE_END = 2,
    };

    
}