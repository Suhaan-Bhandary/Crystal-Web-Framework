#ifndef INCLUDED_JSON_TOKEN
#define INCLUDED_JSON_TOKEN

#include <string>
#include <variant>

namespace Crystal::Json {
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
    UNSIGNED_NUMBER,
    FRACTION,

    // Keywords
    TRUE,
    FALSE,
    NULL_TOKEN,

    // End of line
    EOF_TOKEN,

    // Invalid
    INVALID
};

struct Token {
    TokenType type;
    int lexemeStart, lexemeLength;
    std::variant<std::string, unsigned long long, long long, double>
        literal;  // actual value of lexeme

    int line;

    // Constructor
    Token(TokenType type, int lexemeStart, int lexemeLength,
          const std::string &literal, int line);
    Token(TokenType type, int lexemeStart, int lexemeLength, double literal,
          int line);
    Token(TokenType type, int lexemeStart, int lexemeLength, long long literal,
          int line);
    Token(TokenType type, int lexemeStart, int lexemeLength,
          unsigned long long literal, int line);
    Token(TokenType type, int lexemeStart, int lexemeLength, int line);
};
}  // namespace Crystal::Json

#endif
