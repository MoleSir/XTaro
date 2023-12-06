#pragma once 

#include "jsontoken.hh"

#include <string>
#include <list>
#include <exception>

namespace xtaro::parse
{

    class JsonScanner
    {
    public:
        JsonScanner(std::string content) noexcept:
            _content{std::move(content)},
            _startIndex{0},
            _currentIndex{0} {}

        std::list<JsonToken> scan();

    private:
        void scanToken();

        void scanNumberToken();
        void scanStringToken();
        void scanNullToken();
        void scanTrueToken();
        void scanFalseToken();
    
    private:

        char advanceChar();
        
        char peekChar() noexcept
        { return this->_content[this->_currentIndex]; }

        bool isEnd() const noexcept
        { return this->_currentIndex >= this->_content.size(); }

        const char* startPointer() const noexcept
        { return &(this->_content[this->_startIndex]); }

    private:
        void addToken(JsonTokenType type) 
        { this->_tokens.emplace_back(type); }

        void addToken(bool value)
        { this->_tokens.emplace_back(value); }

        void addToken(int value)
        { this->_tokens.emplace_back(value); }

        void addToken(double value)
        { this->_tokens.emplace_back(value); }

        void addToken(std::string value)
        { this->_tokens.emplace_back(std::move(std::move(value))); } 

    private:
        std::string _content;
        int _startIndex;
        int _currentIndex;
        std::list<JsonToken> _tokens;
    };

}