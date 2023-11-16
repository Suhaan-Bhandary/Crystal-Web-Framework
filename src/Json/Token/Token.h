#ifndef INCLUDED_JSON_TOKEN
#define INCLUDED_JSON_TOKEN

#include <string>
#include <variant>

namespace Json {
enum TokenType {
    // Single Character Token
    COMMA,
    COLON,
    OPEN_CURL_BRACKET,
    CLOSE_CURL_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,

    // Identifier
    STRING,
    NUMBER,
    FRACTION,

    // Keywords
    TRUE,
    FALSE,
    NULL_TOKEN,

    // End of line
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;  // the string itself
    std::variant<std::string, long long, double>
        literal;  // actual value of lexeme

    int line;

    // Constructor
    Token(TokenType type, const std::string &lexeme, const std::string &literal,
          int line);
    Token(TokenType type, const std::string &lexeme, double literal, int line);
    Token(TokenType type, const std::string &lexeme, long long literal,
          int line);
    Token(TokenType type, const std::string &lexeme, int line);
};
}  // namespace Json

#endif