#pragma once

#include <string>
#include <vector>
#include <map>

namespace xtaro::parse
{
    enum class JsonType
    {
        NONE = 0,  //!< null type 
        BOOL,      //!< bool type
        INT,       //!< int type
        DOUBLE,    //!< double type
        STRING,    //!< string type
        ARRAY,     //!< array type
        OBJECT     //!< object type
    };


    //! \brief Json class
    class Json
    {
        using Iterator = std::vector<Json>::iterator;

    public:
        static Json loadFromFile(const std::string& filepath);

    public:
        //! \name Construction functions of Json objects
        Json() noexcept;
        Json(const bool value) noexcept;
        Json(const int value) noexcept;
        Json(const double value) noexcept;
        Json(const char* value) noexcept;
        Json(std::string value) noexcept;
        Json(const JsonType type) noexcept;
        Json(const Json& other) noexcept;
        ~Json() noexcept { this->clear(); }

        //! \name operators of a Json object
        Json& operator = (const Json& other) noexcept;
        bool operator == (const Json& other) const noexcept;
        bool operator != (const Json& other) const noexcept;

        //! \name Conversion functions and operators, allow assign base value by Json object, 
        //! like: 
        //! Json v(12);
        //! int i = v;
        //! the operators just call the "AsXX" function 
        //! \note the "asXX" functions will check the Json Type, 
        //! if the Type is not judge, throw a std::logic_error exception

        bool asBool() const;
        operator bool() const { return this->asBool(); };

        int asInt() const;
        operator int() const { return this->asInt(); }

        double asDouble() const;
        operator double() const { return this->asDouble(); }
        
        std::string asString() const;
        operator std::string() const { return this->asString(); }

        //! \brief operator of [], for array type Json
        //! \note this operator can use for add Json object to the Json array, 
        Json& operator [] (const int index);

        //! \brief Append a json object, for array type Json
        void append(const Json& other); 

        //! \brief Check whether the index is illage 
        bool has(int index) const;

        //! \brief Check array size
        int size() const;

        //! \brief Remove the Json object of index
        //! \note The function will check whether index out of range, so noexcept
        void remove(int index) noexcept;

        //! \name Functions and operators of Object Type 
        //! \brief Get value by key
        Json& operator [] (const char* key);
        Json& operator [] (const std::string& key);

        //! \brief Check wether the key is in the map<string, Json>
        bool has(const char* key) const;
        bool has(const std::string& key) const;

        //! \brief Remove the key-value pair in map
        void remove(const char* key) noexcept;
        void remove(const std::string& key) noexcept;

        //! \name Check json type
        bool isNull() const noexcept { return this->_type == JsonType::NONE; }
        bool isBool() const noexcept { return this->_type == JsonType::BOOL; }
        bool isInt() const noexcept { return this->_type == JsonType::INT; }    
        bool isDouble() const noexcept { return this->_type == JsonType::DOUBLE; }  
        bool isString() const noexcept { return this->_type == JsonType::STRING; } 
        bool isArray() const noexcept { return this->_type == JsonType::ARRAY; } 
        bool isObject() const noexcept { return this->_type == JsonType::OBJECT; } 


        //! \name Usefull functions 
        std::string toString() const noexcept;

        //! \brief Clear the memory in Json, will recursive to claer all the Json in array or map
        void clear() noexcept;

    private:
        //! \brief Copy a Json object to this
        void copy(const Json& other) noexcept;

    private:
        //! \brief A union a store the data in json.
        //! Each Json object only has one type, so use union to save space
        //! A Json objet maybe: 
        //! Bool : true or fasle
        //! Number : 1.2, -23, 12.34, 0.321
        //! String : "name"
        //! Array : [jsonObj1, jsonObj2, ...]
        //! Object : {key1 : jsonObj1, key2 : jsonObj2, key3 : jsonObj3, ...}
        union JsonValue
        {
            bool _bool;
            int _int;
            double _double;
            std::string* _string;  
            std::vector<Json>* _array;
            std::map<std::string, Json>* _object;
            unsigned long long _value;
        };

        JsonType _type;
        JsonValue _value;
        mutable bool _isClear;
    };

}
