#include "Token.h"

Json::Token::Token(TokenType type, const std::string &lexeme,
                   const std::string &literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
};

Json::Token::Token(TokenType type, const std::string &lexeme, double literal,
                   int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
};

Json::Token::Token(TokenType type, const std::string &lexeme, long long literal,
                   int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
};

Json::Token::Token(TokenType type, const std::string &lexeme, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->line = line;
};
