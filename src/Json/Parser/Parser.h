#ifndef INCLUDED_JSON_PARSER
#define INCLUDED_JSON_PARSER

#include <memory>
#include <string>

#include "../Node/Node.h"
#include "../Token/Token.h"

namespace Json {
class Parser {
   public:
    Node *parse(const char *jsonString, bool isFile);

   private:
    int start, current;
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