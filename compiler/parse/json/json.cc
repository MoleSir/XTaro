#include "json.hh"
#include "jsonscanner.hh"
#include "jsonparser.hh"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

namespace xtaro::parse
{

    Json Json::loadFromFile(const std::string& filepath)
    {
        // Read json file content
        std::ifstream file{filepath};
        file.seekg(0, std::ios::end);
        std::size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string content(fileSize + 1, '\0');
        content.resize(fileSize);
        file.read(const_cast<char*>(content.data()), fileSize);
        // std::cout << content.c_str() << '\n';

        // Scan file
        std::list<JsonToken> tokens = JsonScanner{std::move(content)}.scan();

        // Parser file
        return JsonParser(std::move(tokens)).parse();
    }

    Json::Json() noexcept 
        : _type(JsonType::NONE), _value{}, _isClear(true) {}

    Json::Json(const bool value) noexcept
        : _type(JsonType::BOOL), _value{}, _isClear(false)
    {
        this->_value._bool = value;
    }

    Json::Json(const int value) noexcept
        : _type(JsonType::INT), _value{}, _isClear(false)
    {
        this->_value._int = value;
    }

    Json::Json(const double value) noexcept
        : _type(JsonType::DOUBLE), _value{}, _isClear(false)
    {
        this->_value._double = value;
    }

    Json::Json(const char* value) noexcept
        : _type(JsonType::STRING), _value{}, _isClear(false)
    {
        try
        {
            this->_value._string = new std::string(value);
        }
        catch (std::bad_alloc exception)
        {
            std::cout << exception.what() << std::endl;
            this->_type = JsonType::NONE;
            this->_isClear = true; 
        }
    }

    Json::Json(std::string value) noexcept
        : _type(JsonType::STRING), _value{}, _isClear(false)
    {  
        try
        {
            this->_value._string = new std::string(std::move(value));
        }
        catch (std::bad_alloc exception)
        {
            std::cout << exception.what() << std::endl;
            this->_type = JsonType::NONE;
            this->_isClear = true; 
        } 
    }

    Json::Json(const JsonType type) noexcept
        : _type(type), _value{}, _isClear(false)
    {
        try
        {
            switch (type)
            {
            case JsonType::NONE:
            case JsonType::BOOL:
            case JsonType::INT:
            case JsonType::DOUBLE:
                this->_value._value = 0x0;
                break;
            case JsonType::STRING:
                this->_value._string = new std::string("");
                break;
            case JsonType::ARRAY:
                this->_value._array = new std::vector<Json>();
                break;
            case JsonType::OBJECT:
                this->_value._object = new std::map<std::string, Json>();
            default:
                break;
            }
        }
        catch (std::bad_alloc exception)
        {
            std::cout << exception.what() << std::endl;
            this->_type = JsonType::NONE;
            this->_isClear = true;
        }
    }

    Json& Json::operator = (const Json& other) noexcept
    {
        if (other._isClear == true) return *this;

        this->clear();
        this->copy(other);
        return *this;
    }

    Json::Json(const Json& other) noexcept
        : _type(other._type), _value{}
    {
        if (other._isClear == true) return;
        this->copy(other);
    }

    //! \brief conversion
    bool Json::asBool() const
    {
        // check type
        if (this->_type != JsonType::BOOL) 
            throw std::logic_error("type error, not bool value");
        return this->_value._bool;
    }

    int Json::asInt() const
    {
        if (this->_type != JsonType::INT)
            throw std::logic_error("tyep error, not int value");
        return this->_value._int;
    }

    double Json::asDouble() const
    {
        if (this->_type != JsonType::DOUBLE)
            throw std::logic_error("type error, not double value");
        return this->_value._double;
    }

    std::string Json::asString() const
    {
        if (this->_type != JsonType::STRING)
            throw std::logic_error("type error, not std::string value");
        return *(this->_value._string);
    }

    void Json::copy(const Json& other) noexcept
    {
        this->_type = other._type;
        switch (this->_type)
        {
        case JsonType::NONE:
            break;
        case JsonType::BOOL:
            this->_value._bool = other._value._bool;
            break;
        case JsonType::INT:
            this->_value._int = other._value._int;
            break;
        case JsonType::DOUBLE:
            this->_value._double = other._value._double;
            break;
        case JsonType::STRING:
            this->_value._string = other._value._string;
            break;
        case JsonType::ARRAY:
            this->_value._array = other._value._array;
            break;
        case JsonType::OBJECT:
            this->_value._object = other._value._object;
        default:
            break;
        }

        this->_isClear = false;
        other._isClear = true;
    }

    void Json::clear() noexcept
    {
        if (this->_isClear == true) return;

        switch (this->_type)
        {
        case JsonType::NONE:
            break;
        case JsonType::BOOL:
            this->_value._bool = false;
            break;
        case JsonType::INT:
            this->_value._int = 0;
            break;
        case JsonType::DOUBLE:
            this->_value._double = 0.0;
            break;
        case JsonType::STRING:
            delete this->_value._string;
            break;
        case JsonType::ARRAY:
            delete this->_value._array;
            break;
        case JsonType::OBJECT:
            delete this->_value._object;
            break;
        default:
            break;
        }   

        this->_isClear = true;
        this->_type = JsonType::NONE;
    }

    Json& Json::operator [] (const int index)
    {
        if (this->_type != JsonType::ARRAY)
            throw std::logic_error("TODO");

        if (index < 0) 
            throw std::logic_error("array[] index < 0");
        
        return (this->_value._array)->at(index);
    }

    void Json::append(const Json& other)
    {
        // must be a array type
        if (this->_type != JsonType::ARRAY)
            return;
        this->_value._array->push_back(other);
    }

    std::string Json::toString() const noexcept
    {
        std::stringstream ss;
    
        switch (this->_type)
        {
        case JsonType::NONE:
            ss << "null";
            break;
        case JsonType::BOOL:
            ss << (this->_value._bool == true ? "true" : "false"); 
            break;
        case JsonType::INT:
            ss << this->_value._int;
            break;
        case JsonType::DOUBLE:
            ss << this->_value._double;
            break;
        case JsonType::STRING:
            ss << '\"' << *(this->_value._string) << '\"';
            break;
        case JsonType::ARRAY:
            ss << '[';
            for (auto iter = (this->_value._array)->cbegin(); iter != (this->_value._array)->cend(); ++iter)
            {
                if (iter != (this->_value._array->cbegin()))
                    ss << ", ";
                ss << iter->toString();
            }
            ss << ']';
            break;
        case JsonType::OBJECT:
            ss << '{';
            for (auto iter = (this->_value._object)->cbegin(); iter != (this->_value._object)->cend(); ++iter)
            {
                if (iter != (this->_value._object)->cbegin())
                    ss << ", ";
                ss << '\"' << iter->first << '\"' << ": " << iter->second.toString();
            }
            ss << '}';
            break;
        default:
            break;
        }

        return ss.str();
    }

    Json& Json::operator [] (const char* key)
    {
        std::string keyStr{key};
        return (*this)[keyStr];
    }

    Json& Json::operator [] (const std::string& key)
    {
        if (this->_type != JsonType::OBJECT)
        {
            this->clear();
            this->_type = JsonType::OBJECT;
            this->_value._object = new std::map<std::string, Json>();
        }
        // return a reference
        return (*(this->_value)._object)[key];
    }

    bool Json::operator == (const Json& other) const noexcept
    {
        if (this->_type != other._type) return false;

        switch (this->_type)
        {
        case JsonType::NONE:
            return true;
            break;
        case JsonType::BOOL:
            return this->_value._bool == other._value._bool;
            break;
        case JsonType::INT:
            return this->_value._int == other._value._int;
            break;
        case JsonType::DOUBLE:
            return this->_value._double == other._value._double;
            break;
        case JsonType::STRING:
            return *(this->_value._string) == *(other._value._string);
            break;
        case JsonType::ARRAY:
            return this->_value._array == other._value._array;
            break;
        case JsonType::OBJECT:
            return this->_value._object == other._value._object;
            break;
        default:
            break;
        }  
        return false;
    }

    bool Json::operator != (const Json& other) const noexcept 
    { return !(*this == other); }

    bool Json::has(int index) const
    {
        if (this->_type != JsonType::ARRAY) return false;
        return (index >= 0 && index < this->_value._array->size());
    }

    bool Json::has(const char* key) const
    {
        std::string keyStr{key};
        return this->has(keyStr);
    }

    bool Json::has(const std::string& key) const
    {
        if (this->_type != JsonType::OBJECT) return false;
        return ((this->_value._object->find(key)) != this->_value._object->end());
    }

    int Json::size() const
    {
        if (this->isObject()) return this->_value._object->size();
        if (this->isArray()) return this->_value._array->size();
        return -1;
    }

    void Json::remove(int index)  noexcept
    {
        if (this->_type != JsonType::ARRAY) return;

        int size = (this->_value._array)->size();
        if (index < 0 || index >= size) return;

        // clear the Json object
        (this->_value._array)->at(index).clear();
        (this->_value._array)->erase((this->_value._array)->begin() + index);
    }

    void Json::remove(const char* key) noexcept
    {
        std::string keyStr{key};
        this->remove(keyStr);
    }

    void Json::remove(const std::string& key) noexcept
    {
        auto iter = (this->_value._object)->find(key);
        if (iter != (this->_value._object)->end())
        {
            // Clear the Json object
            (*(this->_value._object))[key].clear();
            (this->_value._object)->erase(iter);
        }
    }

}
