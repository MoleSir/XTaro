#include "json.hh"
#include "jsonscanner.hh"
#include "jsonparser.hh"

#include <allocator/allocator.hh>
#include <util/format.hh>
#include <util/file.hh>
#include <debug/debug.hh>


#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

namespace xtaro::parse
{

    const char* Json::_jsonTypeString[JsonType::COUNT] = {
        "Null", "Boolean", "Integer", "Decimal", "String", "Array", "Object",
    };

    Json Json::loadFromFile(const std::string& filepath)
    {
        // Read json file content
        std::ifstream file{filepath};
        if (!file.is_open())
            debug->error("Load json file", util::format("File '%s' does not exit.", filepath.c_str()));

        std::string content {util::readFile(file)};
        file.close();

        // Scan file
        std::list<JsonToken> tokens = JsonScanner{std::move(content)}.scan();

        // Parser file
        return JsonParser(std::move(tokens)).parse();
    }

    Json::Json() noexcept : 
        _type(JsonType::EMPTY), _value{}, _invalid(true) {}

    Json::Json(const bool value) noexcept : 
        _type(JsonType::BOOLEAN), _value{}, _invalid(false)
    {
        this->_value._bool = value;
    }

    Json::Json(const int value) noexcept : 
        _type(JsonType::INTEGER), _value{}, _invalid(false)
    {
        this->_value._int = value;
    }

    Json::Json(const double value) noexcept : 
        _type(JsonType::DECIMAL), _value{}, _invalid(false)
    {
        this->_value._double = value;
    }

    Json::Json(const char* value) : 
        _type(JsonType::STRING), _value{}, _invalid(true)
    {
        // this->_value._string = new std::string(value);
        this->_value._string = Allocator::alloc<std::string>(value);
        this->_invalid = false;
    }

    Json::Json(std::string value) : 
        _type(JsonType::STRING), _value{}, _invalid(true)
    {
        // this->_value._string = new std::string(std::move(value));
        this->_value._string = Allocator::alloc<std::string>(std::move(value));
        this->_invalid = false;
    }

    Json::Json(const JsonType type) : 
        _type(type), _value{}, _invalid(true)
    {
        switch (type)
        {
        case JsonType::NIL:
            this->_value._value = 0x0ULL;
            break;
        case JsonType::BOOLEAN:
            this->_value._bool = false;
            break;
        case JsonType::INTEGER:
            this->_value._int = 0;
            break;
        case JsonType::DECIMAL:
            this->_value._double = 0.0;
            break;
        case JsonType::STRING:
            // this->_value._string = new std::string();
            this->_value._string = Allocator::alloc<std::string>();
            break;
        case JsonType::ARRAY:
            // this->_value._array = new std::vector<Json>();
            this->_value._array = Allocator::alloc<std::vector<Json>>();
            break;
        case JsonType::OBJECT:
            // this->_value._object = new std::map<std::string, Json>();
            this->_value._object = Allocator::alloc<std::map<std::string, Json>>();
            break;
        default:
            break;
        }
        this->_invalid = false;
    }

    Json::Json(const Json& other) : 
        _type(other._type), _value{}, _invalid(true)        
    {
        // Copy as Move
        this->move(other);
    }

    Json::Json(Json&& other) :
        _type(other._type), _value{}, _invalid(true)   
    {
        this->move(other);
    }

    Json& Json::operator = (const Json& other)
    {
        other.ensureValid();
        if (this == &other) return *this;

        this->clear();
        this->move(other);
        return *this;
    }

    Json& Json::operator = (Json&& other)
    {
        other.ensureValid();
        if (this == &other) return *this;

        this->clear();
        this->move(other);
        return *this;
    }

    bool Json::operator == (const Json& other) const
    {
        this->ensureValid();
        other.ensureValid();
        if (this->_type != other._type) return false;

        switch (this->_type)
        {
        case JsonType::NIL:
            return true;
            break;
        case JsonType::BOOLEAN:
            return this->_value._bool == other._value._bool;
            break;
        case JsonType::INTEGER:
            return this->_value._int == other._value._int;
            break;
        case JsonType::DECIMAL:
            return this->_value._double == other._value._double;
            break;
        case JsonType::STRING:
            return *(this->_value._string) == *(other._value._string);
            break;
        case JsonType::ARRAY:
            return *(this->_value._array) == *(other._value._array);
            break;
        case JsonType::OBJECT:
            return *(this->_value._object) == *(other._value._object);
            break;
        default:
            break;
        }  
        return false;
    }

    bool Json::operator != (const Json& other) const 
    { return !(*this == other); }

    //! \brief conversion
    bool Json::asBoolean() const
    {
        this->ensureJsonTyep(JsonType::BOOLEAN);
        return this->_value._bool;
    }

    int Json::asInteger() const
    {
        this->ensureJsonTyep(JsonType::INTEGER);
        return this->_value._int;
    }

    double Json::asDecimal() const
    {
        this->ensureJsonTyep(JsonType::DECIMAL);
        return this->_value._double;
    }

    std::string& Json::asString() const
    {
        this->ensureJsonTyep(JsonType::STRING);
        return *(this->_value._string);
    }

    void Json::move(const Json& other)
    {
        other.ensureValid();

        this->_type = other._type;
        this->_value = other._value;

        this->_invalid = false;
        other._invalid = true;
    }

    Json Json::clone() const
    {
        this->ensureValid();

        switch (this->_type)
        {
        case JsonType::NIL:
            return Json();
        case JsonType::BOOLEAN:
            return Json(this->_value._bool);
        case JsonType::INTEGER:
            return Json(this->_value._int);
        case JsonType::DECIMAL:
            return Json(this->_value._double); 
        case JsonType::STRING:
            return Json(this->_value._string);
        case JsonType::ARRAY:
        {
            Json json(JsonType::ARRAY);
            for (int i = 0; i < this->size(); ++i)
                json.append(this->_value._array->at(i).clone());

            return json;
        }
        case JsonType::OBJECT:
        {
            Json json(JsonType::OBJECT);
            for (auto iter = this->_value._object->begin(); 
                 iter != this->_value._object->end(); 
                 ++iter)
                json.insert(iter->first, iter->second);

            return json;
        }
        default:
            return Json();
        }
    }

    void Json::clear() noexcept
    {
        if (this->_invalid == true) return;

        switch (this->_type)
        {
        case JsonType::NIL:
        case JsonType::BOOLEAN:
        case JsonType::INTEGER:
        case JsonType::DECIMAL:
            break;
        case JsonType::STRING:
            Allocator::free(this->_value._string);
            break;
        case JsonType::ARRAY:
            Allocator::free(this->_value._array);
            break;
        case JsonType::OBJECT:
            Allocator::free(this->_value._object);
            break;
        default:
            break;
        }   

        this->_invalid = true;
    }

    Json& Json::operator [] (const int index)
    {
        this->ensureJsonTyep(JsonType::ARRAY);
        if (index < 0) 
            debug->error("Json array", "Array index < 0.");
        
        return (this->_value._array)->at(index);
    }

    void Json::append(const Json& other)
    {
        this->ensureJsonTyep(JsonType::ARRAY);
        this->_value._array->emplace_back(other);
    }

    Json& Json::operator [] (const std::string& key)
    {
        this->ensureJsonTyep(JsonType::OBJECT);
        return (*(this->_value)._object)[key];
    }

    Json& Json::operator [] (const char* key)
    {
        return this->operator[](std::string(key));
    }

    Json Json::get(const std::string& key)
    {
        this->ensureJsonTyep(JsonType::OBJECT);
        auto iter{ this->_value._object->find(key) };
        if (iter == this->_value._object->end())
            return Json{};
        return iter->second;
    }

    void Json::insert(std::string key, const Json& value)
    {
        this->ensureJsonTyep(JsonType::OBJECT);
        this->_value._object->emplace(std::move(key), value);
    }

    bool Json::has(int index) const
    {
        this->ensureJsonTyep(JsonType::ARRAY);

        if (this->_type != JsonType::ARRAY) return false;
        return (index >= 0 && index < this->_value._array->size());
    }

    bool Json::has(const std::string& key) const
    {
        this->ensureJsonTyep(JsonType::OBJECT);
        return ((this->_value._object->find(key)) != this->_value._object->end());
    }

    int Json::size() const
    {
        this->ensureValid();

        if (this->isObject()) return this->_value._object->size();
        if (this->isArray()) return this->_value._array->size();
        debug->error("Json type", util::format("%s has no size.", Json::_jsonTypeString[this->_type]));
        return -1;
    }

    void Json::remove(int index)
    {
        this->ensureJsonTyep(JsonType::ARRAY);

        int size = (this->_value._array)->size();
        if (index < 0 || index >= size) return;

        // clear the Json object
        (this->_value._array)->at(index).clear();
        (this->_value._array)->erase((this->_value._array)->begin() + index);
    }

    void Json::remove(const std::string& key)
    {
        this->ensureJsonTyep(JsonType::OBJECT);

        auto iter = (this->_value._object)->find(key);
        if (iter != (this->_value._object)->end())
        {
            // Clear the Json object
            (*(this->_value._object))[key].clear();
            (this->_value._object)->erase(iter);
        }
    }
    
    void Json::writeToFile(const std::string& filename) const
    {
        this->ensureValid();
        std::ofstream file(filename);
        file << this->toString();
        file.close();
    }

    void Json::ensureValid() const
    {
        if (this->_invalid)
            debug->error("Json", "Use an invalid json.");
    }

    void Json::ensureJsonTyep(JsonType type) const
    {
        this->ensureValid();

        if (type != this->_type)
            debug->error(
                "Json type",
                util::format("Json Type mismatch, excepted %s but got %s",
                             Json::_jsonTypeString[type], 
                             Json::_jsonTypeString[this->_type])
            );
    }

    std::string Json::doToString(std::size_t indent, bool hasName, bool isObjectValue) const
    {
        this->ensureValid();

        std::stringstream ss;

        auto printSpace = [&ss] (std::size_t indent) {
            for (std::size_t i = 0; i < indent; ++i)
                ss << "    ";
        };

        switch (this->_type)
        {
        case JsonType::NIL:
            ss << "null";
            break;
        case JsonType::BOOLEAN:
            ss << (this->_value._bool == true ? "true" : "false"); 
            break;
        case JsonType::INTEGER:
            ss << this->_value._int;
            break;
        case JsonType::DECIMAL:
            ss << this->_value._double;
            break;
        case JsonType::STRING:
            ss << '\"' << *(this->_value._string) << '\"';
            break;
        case JsonType::ARRAY:
            if (!hasName)  printSpace(indent);
            ss << '[' << '\n';

            for (auto iter = (this->_value._array)->cbegin(); 
                iter != (this->_value._array)->cend(); 
                ++iter)
            {
                if (iter != (this->_value._array->cbegin()))
                    ss << ',' << '\n';
                printSpace(indent + 1);
                ss << iter->doToString(indent + 1, true, false);
            }

            ss << '\n';
            printSpace(indent);
            ss << ']';
            break;
        case JsonType::OBJECT:
            if (!hasName) printSpace(indent);
            ss << '{' << '\n';

            for (auto iter = (this->_value._object)->cbegin(); 
                iter != (this->_value._object)->cend(); 
                ++iter)
            {
                if (iter != (this->_value._object)->cbegin())
                    ss << ',' << '\n';
                printSpace(indent + 1);
                ss << '\"' << iter->first << '\"' << " : " 
                   << iter->second.doToString(indent + 1, true, true);
            }

            ss << '\n';
            printSpace(indent);
            ss << '}';
            break;
        default:
            break;
        }

        return ss.str();
    }
}
