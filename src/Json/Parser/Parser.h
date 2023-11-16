#ifndef INCLUDED_JSON_PARSER
#define INCLUDED_JSON_PARSER

#include <memory>
#include <string>

#include "../Node/Node.h"
#include "../Token/Token.h"

namespace Json {
class Parser {
   public:
    Parser() = delete;
    Parser(const char *jsonString, bool isFile = false);
    Node *parse();

   private:
    const char *source;
    int start, current;
    bool readFromFile;
    std::vector<Token> tokens;

    Node *parseTokens();
    Node *parseObjectTokens();
    Node *parseArrayTokens();

    Token &peek();
    Token &advance();
    bool isAtEnd();
};

}  // namespace Json

#endif