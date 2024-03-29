#include "Token.h"

Crystal::Json::Token::Token(TokenType type, int lexemeStart, int lexemeLength,
                            const std::string &literal, int line) {
    this->type = type;
    this->lexemeStart = lexemeStart;
    this->lexemeLength = lexemeLength;
    this->literal = literal;
    this->line = line;
};

Crystal::Json::Token::Token(TokenType type, int lexemeStart, int lexemeLength,
                            double literal, int line) {
    this->type = type;
    this->lexemeStart = lexemeStart;
    this->lexemeLength = lexemeLength;
    this->literal = literal;
    this->line = line;
};

Crystal::Json::Token::Token(TokenType type, int lexemeStart, int lexemeLength,
                            long long literal, int line) {
    this->type = type;
    this->lexemeStart = lexemeStart;
    this->lexemeLength = lexemeLength;
    this->literal = literal;
    this->line = line;
};

Crystal::Json::Token::Token(TokenType type, int lexemeStart, int lexemeLength,
                            unsigned long long literal, int line) {
    this->type = type;
    this->lexemeStart = lexemeStart;
    this->lexemeLength = lexemeLength;
    this->literal = literal;
    this->line = line;
};

Crystal::Json::Token::Token(TokenType type, int lexemeStart, int lexemeLength,
                            int line) {
    this->type = type;
    this->lexemeStart = lexemeStart;
    this->lexemeLength = lexemeLength;
    this->line = line;
};
