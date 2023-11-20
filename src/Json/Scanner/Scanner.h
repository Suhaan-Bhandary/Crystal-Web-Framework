#ifndef INCLUDED_JSON_SCANNER
#define INCLUDED_JSON_SCANNER

#include <fstream>
#include <string>
#include <vector>

#include "../Token/Token.h"

namespace Json {
class Scanner {
   public:
    Scanner() = delete;
    ~Scanner();
    Scanner(const char* rawJsonString, bool readFromFile);

    std::vector<Token>& scanTokens();

   private:
    bool readFromFile;
    int start, current, line;
    char* source;
    std::vector<Token> tokens;

    // Functions
    bool scanToken();
    bool stringToken();
    bool numberToken(char firstChar);
    bool identifierToken();

    void addToken(TokenType type);
    void addToken(TokenType type, std::string literal);
    void addToken(TokenType type, double literal);
    void addToken(TokenType type, long long literal);

    char advance();
    bool match(char expected);
    char peek();

    bool isAtEnd();
    bool isNextFourCharacterDigit();
    bool isHexCharacter(char ch);
    bool isNumberStart(char ch);
    bool isAlpha(char ch);

    char* readFile(const char* path);
};
}  // namespace Json

#endif