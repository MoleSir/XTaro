#pragma once

#include <string>
#include <vector>
#include <map>

namespace xtaro::parse
{
    enum JsonType
    {
        EMPTY,
        NIL,
        BOOLEAN,
        INTEGER,
        DECIMAL,
        STRING,
        ARRAY,
        OBJECT,
        COUNT,
    };

    class Json
    {
        static const char* _jsonTypeString[JsonType::COUNT];
    
    public:
        static Json loadFromFile(const std::string& filepath);

    public:
        // Construction functions
        Json() noexcept;
        Json(const bool value) noexcept;
        Json(const int value) noexcept;
        Json(const double value) noexcept;
        Json(const char* value);
        Json(std::string value);
        Json(const JsonType type);

        Json(const Json& other);
        Json(Json&& other);
        ~Json() noexcept { this->clear(); }

        // Assign, Move, Compare
        Json& operator = (const Json& other);
        Json& operator = (Json&& other);
        bool operator == (const Json& other) const;
        bool operator != (const Json& other) const;

        // Conversion functions and operators
        bool asBoolean() const;
        operator bool() const { return this->asBoolean(); };

        int asInteger() const;
        operator int() const { return this->asInteger(); }

        double asDecimal() const;
        operator double() const { return this->asDecimal(); }
        
        std::string& asString() const;
        operator std::string() const { return this->asString(); }

        // Operator of [], for array type Json
        Json& operator [] (const int index);

        // Append a json object, for array type Json
        void append(const Json& other); 

        // Check whether the index is illage 
        bool has(int index) const;

        // Remove the Json object of index
        void remove(int index);

        // Functions and operators of Object Type
        Json& operator [] (const std::string& key);
        Json& operator [] (const char* key);

        // If no 'key', return a cleared json
        Json get(const std::string& key);

        void insert(std::string key, const Json& value);

        // Check wether the key is in the map<string, Json>
        bool has(const std::string& key) const;

        // Remove the key-value pair in map
        void remove(const std::string& key);

        // Check size, use both Array and Object
        int size() const;

        // Check json type
        bool invalid() const noexcept
        { return this->_invalid; }

        bool isNull() const noexcept 
        { return !this->_invalid && this->_type == JsonType::NIL; }
        
        bool isBool() const noexcept 
        { return !this->_invalid && this->_type == JsonType::BOOLEAN; }
        
        bool isInt() const noexcept 
        { return !this->_invalid && this->_type == JsonType::INTEGER; }    
        
        bool isDouble() const noexcept 
        { return !this->_invalid && this->_type == JsonType::DECIMAL; }  
        
        bool isString() const noexcept 
        { return !this->_invalid && this->_type == JsonType::STRING; } 
        
        bool isArray() const noexcept 
        { return !this->_invalid && this->_type == JsonType::ARRAY; } 
        
        bool isObject() const noexcept 
        { return !this->_invalid && this->_type == JsonType::OBJECT; } 

        // Usefull functions 
        std::string toString() const
        { return this->doToString(); }

        void writeToFile(const std::string& filename) const;

    public:
        // Clear the memory in Json, will recursive to claer all the Json in array or map
        void clear() noexcept;

        // Move a Json object to this
        void move(const Json& other);

        // Clone this json(not move!)
        Json clone() const;

    private:
        void ensureValid() const;
        void ensureJsonTyep(JsonType type) const;

        std::string doToString(std::size_t indent = 0, bool hasName = false, bool isObjectValue = false) const;

    private:
        // A union a store the data in json.
        // Each Json object only has one type, so use union to save space
        // A Json objet maybe: 
        // Bool : true or fasle
        // Number : 1.2, -23, 12.34, 0.321
        // String : "name"
        // Array : [jsonObj1, jsonObj2, ...]
        // Object : {key1 : jsonObj1, key2 : jsonObj2, key3 : jsonObj3, ...}
        union JsonValue
        {
            JsonValue(): _value{0ULL} {}
            bool _bool;
            int _int;
            double _double;
            std::string* _string;  
            std::vector<Json>* _array;
            std::map<std::string, Json>* _object;
            std::uint64_t _value;
        };

        JsonType _type;
        JsonValue _value;

        mutable bool _invalid;
    };

}
