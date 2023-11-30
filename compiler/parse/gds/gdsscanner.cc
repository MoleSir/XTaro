#include "gdsscanner.hh"
#include "gdsrecord.hh"
#include "./value/gdsvalue.hh"
#include <list>

namespace xtaro::parser
{

    GDSScanner::GDSScanner(const std::string& gdsFilePath) :
        _bytes{}, _records{}, _index{0}, _ptr{nullptr}, 
        _convertor{
            // File Header Records
            {GDSRecordType::HEADER, GDSScanner::getInt16},
            {GDSRecordType::BGNLIB, GDSScanner::getTimestamp},
            {GDSRecordType::LIBNAME, GDSScanner::getString},
            {GDSRecordType::REFLIBS, GDSScanner::getRefencelibs},
            {GDSRecordType::FONTS, GDSScanner::getFonts},
            {GDSRecordType::ATTRTABLE, GDSScanner::getString},
            {GDSRecordType::GENERATIONS, GDSScanner::getInt16},
            {GDSRecordType::FORMAT, GDSScanner::getInt16},
            {GDSRecordType::MASK, GDSScanner::getString},
            {GDSRecordType::ENDMASKS, GDSScanner::getNull},
            {GDSRecordType::UNITS, GDSScanner::getUints},

            // File Tail Records
            {GDSRecordType::ENDLIB, GDSScanner::getNull},

            // Structure Header Records
            {GDSRecordType::BGNSTR, GDSScanner::getTimestamp},
            {GDSRecordType::STRNAME, GDSScanner::getString},

            // Strcuture Tail Records
            {GDSRecordType::ENDSTR, GDSScanner::getNull},

            // Element Header Records
            {GDSRecordType::BOUNDARY, GDSScanner::getNull},
            {GDSRecordType::PATH, GDSScanner::getNull},
            {GDSRecordType::SREF, GDSScanner::getNull},
            {GDSRecordType::AREF, GDSScanner::getNull},
            {GDSRecordType::TEXT, GDSScanner::getNull},
            {GDSRecordType::NODE, GDSScanner::getNull},
            {GDSRecordType::BOX, GDSScanner::getNull},
            
            // Element Contents Records
            {GDSRecordType::ELFLAGS, GDSScanner::getElementFLags},
            {GDSRecordType::PLEX, GDSScanner::getInt32},
            {GDSRecordType::LAYER, GDSScanner::getInt16},
            {GDSRecordType::DATATYPE, GDSScanner::getInt16},
            {GDSRecordType::XY, GDSScanner::getXY},
            {GDSRecordType::PATHTYPE, GDSScanner::getInt16},
            {GDSRecordType::WIDTH, GDSScanner::getInt32},
            {GDSRecordType::SNAME, GDSScanner::getString},
            {GDSRecordType::MAG, GDSScanner::getDouble},
            {GDSRecordType::ANGLE, GDSScanner::getDouble},
            {GDSRecordType::COLROW, GDSScanner::getColumnRow},
            {GDSRecordType::TEXTTYPE, GDSScanner::getInt16},
            {GDSRecordType::PRESENTATION, GDSScanner::getInt16},
            {GDSRecordType::STRING, GDSScanner::getString},
            {GDSRecordType::NODETYPE, GDSScanner::getInt16},
            {GDSRecordType::BOXTYPE, GDSScanner::getInt16}
        }
    {
        // Get file size
        std::ifstream file(gdsFilePath, std::ios::binary);
        file.seekg(0, std::ios::end);
        std::size_t filesize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file content
        this->_bytes.resize(filesize);
        file.read(reinterpret_cast<char*>(this->_bytes.data()), filesize);
        file.close();

        this->_ptr = this->_bytes.data();
    }

    std::list<GDSRecord>&& GDSScanner::scan()
    {
        while (this->isEnd() == false)
            this->scanRecord();
        
        return std::move(this->_records);
    }

    void GDSScanner::scanRecord()
    {
        // Read size
        std::int16_t size = GDSScanner::scanInt16(this->_ptr);
        // Read type
        GDSRecordType type = static_cast<GDSRecordType>(GDSScanner::scanInt16(this->_ptr + 2));
        // Read content
        void* instance = this->_convertor[type](
            this->_ptr + 4, size
        );

        this->_records.emplace_back(type, size, instance);

        this->_index += size;
        this->_ptr += size;
    }

    std::int16_t GDSScanner::scanInt16(const std::uint8_t* content) noexcept
    {
        std::uint16_t value = ((content[0] << 8) | content[1]);
        return static_cast<std::int16_t>(value);
    }

    std::int32_t GDSScanner::scanInt32(const std::uint8_t* content) noexcept
    {
        std::uint32_t value = (content[0] << 24) | (content[1] << 16) | (content[2] << 8) | content[3];
        return static_cast<std::int16_t>(value);
    }

    double GDSScanner::scanDouble(const std::uint8_t* content) noexcept
    {
        // TODO
        return 0.0;
    }


    void* GDSScanner::getInt16(const std::uint8_t* content, const std::int16_t size)
    {
        std::int16_t* instance = new std::int16_t;
        *instance = GDSScanner::scanInt16(content);
        return instance;
    }

    void* GDSScanner::getInt32(const std::uint8_t* content, const std::int16_t size)
    {
        std::int32_t* instance = new std::int32_t;
        *instance = GDSScanner::scanInt32(content);
        return instance;
    }

    void* GDSScanner::getDouble(const std::uint8_t* content, const std::int16_t size)
    {
        double* instance = new double;
        *instance = GDSScanner::scanDouble(content);
        return instance;
    }

    void* GDSScanner::getTimestamp(const std::uint8_t* content, const std::int16_t size)
    {
        return new GDSTimestamp(
            scanInt16(content),      scanInt16(content + 2),  scanInt16(content + 4),
            scanInt16(content + 6),  scanInt16(content + 8),  scanInt16(content + 10),
            scanInt16(content + 12), scanInt16(content + 14), scanInt16(content + 16),
            scanInt16(content + 18), scanInt16(content + 20), scanInt16(content + 22)
        );
    }

    void* GDSScanner::getString(const std::uint8_t* content, const std::int16_t size)
    {
        return new std::string( reinterpret_cast<const char*>(content) );
    }

    void* GDSScanner::getRefencelibs(const std::uint8_t* content, const std::int16_t size)
    {
        const char* strptr = reinterpret_cast<const char*>(content);
        return new GDSRefenceLibs(strptr, strptr + REFLIBS_STRING_SIZE);
    }

    void* GDSScanner::getFonts(const std::uint8_t* content, const std::int16_t size)
    {
        const char* strptr = reinterpret_cast<const char*>(content);
        return new GDSFonts(
            strptr, strptr + FONTS_STRING_SIZE, 
            strptr + 2 * FONTS_STRING_SIZE, strptr + 3 * FONTS_STRING_SIZE
        );
    }

    void* GDSScanner::getUints(const std::uint8_t* content, const std::int16_t size)
    {
        double userUnits = GDSScanner::scanDouble(content);
        double unitMeters = GDSScanner::scanDouble(content + 8);
        return new GDSUnits(userUnits, unitMeters);
    }

    void* GDSScanner::getXY(const std::uint8_t* content, const std::int16_t size)
    {
        const std::size_t coordinatesSize = size / 8;
        GDSXY* gdsxy = new GDSXY(coordinatesSize);
        
        for (std::size_t i = 0; i < coordinatesSize; ++i)
        {
            const std::uint8_t* xptr = content + (i << 3);
            const std::uint8_t* yptr = xptr + 4;
            gdsxy->addCoordinate(GDSScanner::scanInt32(xptr), GDSScanner::scanInt32(yptr));
        }

        return gdsxy;
    }

    void* GDSScanner::getColumnRow(const std::uint8_t* content, const std::int16_t size)
    {
        return new GDSColumnRow(
            GDSScanner::scanInt16(content), GDSScanner::scanInt16(content + 2)
        );
    }

    void* GDSScanner::getElementFLags(const std::uint8_t* content, const std::int16_t size)
    {
        return new GDSElementFlags(GDSScanner::scanInt16(content));
    }

    void* GDSScanner::getNull(const std::uint8_t* content, const std::int16_t size)
    {
        return nullptr;
    }
}