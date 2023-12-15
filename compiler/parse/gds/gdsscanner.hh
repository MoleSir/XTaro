#pragma once

#include "gdsrecord.hh"

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>

namespace xtaro::parser
{

    class GDSScanner
    {
    public:
        GDSScanner(const std::string& gdsFilePath);

        std::list<GDSRecord>&& scan();

    private:
        void scanRecord();
        bool isEnd() const noexcept 
        { return this->_index >= this->_bytes.size(); }

    private:
        static std::int16_t scanInt16(const std::uint8_t* content) noexcept;
        static std::int32_t scanInt32(const std::uint8_t* content) noexcept;
        static double scanDouble(const std::uint8_t* content) noexcept;

        enum 
        {
            REFLIBS_STRING_SIZE = 45,
            FONTS_STRING_SIZE = 44,
        };

        using GDSRecordConvert = std::function<GDSValue*(const std::uint8_t*, const std::int16_t)>;

        static GDSValue* getInt16(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getInt32(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getDouble(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getTimestamp(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getString(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getRefencelibs(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getFonts(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getUints(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getXY(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getColumnRow(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getElementFLags(const std::uint8_t* content, const std::int16_t size);
        static GDSValue* getNull(const std::uint8_t* content, const std::int16_t size);

    private:
        std::vector<std::uint8_t>  _bytes;
        std::list<GDSRecord>       _records;
        std::size_t                _index;
        std::uint8_t*              _ptr;
        std::map<GDSRecordType, GDSRecordConvert> _convertor;
    };

}
