#pragma once

#include "jsontoken.hh"
#include "json.hh"

#include <string>
#include <list>
#include <exception>

namespace xtaro::parse
{
    class JsonParser
    {
    public:
        JsonParser(std::list<JsonToken> tokens) noexcept:
            _tokens{std::move(tokens)}, _iterator{_tokens.begin()} {}
        
        Json parse();

    private:
        Json parseObject();
        Json parseArray();

    private:
        JsonToken& advanceToken();
        void jumpToken() noexcept
        { this->_iterator++; }

        bool checkTokenType(JsonTokenType type);
        JsonToken& ensureTokenType(JsonTokenType type, const char* message);

        bool isEnd() const noexcept
        { return this->_iterator->type() == JsonTokenType::ENDOFFILE; }

    private:
        std::list<JsonToken> _tokens;
        std::list<JsonToken>::iterator _iterator;
            
    };

}