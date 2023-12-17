#ifndef INCLUDED_JSON_SCANNER
#define INCLUDED_JSON_SCANNER

#include <fstream>
#include <string>

#include "../Token/Token.h"

namespace Crystal::Json {
class Scanner {
   public:
    Scanner() = delete;
    Scanner(const char* source);

    Scanner(const Scanner& otherScanner);
    Scanner& operator=(const Scanner& otherScanner);

    Scanner(Scanner&& otherScanner);
    Scanner& operator=(Scanner&& otherScanner);

    // Public Functions to use in parser
    bool isAtEnd();
    Token advanceToken();
    Token peekToken();

   private:
    int start, current, line;
    const char* source;

    // Functions
    Token scanToken();
    Token stringToken();
    Token numberToken(char firstChar);
    Token identifierToken();

    // trims the characters such as spaces and newline and resets the start and
    // current
    void trimCharacters();

    // Function to create tokens
    inline Token createToken(TokenType type);
    inline Token createToken(TokenType type, const std::string& literal);
    inline Token createToken(TokenType type, double literal);
    inline Token createToken(TokenType type, long long literal);
    inline Token createToken(TokenType type, unsigned long long literal);

    char advance();
    bool match(char expected);
    char peek();

    bool isNextFourCharacterDigit();
    bool isHexCharacter(char ch);
    bool isNumberStart(char ch);
    bool isAlpha(char ch);
};
}  // namespace Crystal::Json

#endif
