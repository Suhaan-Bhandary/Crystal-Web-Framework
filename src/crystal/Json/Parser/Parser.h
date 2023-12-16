#ifndef INCLUDED_JSON_PARSER
#define INCLUDED_JSON_PARSER

#include <memory>
#include <string>

#include "../Node/Node.h"
#include "../Token/Token.h"
#include "../Scanner/Scanner.h"

namespace Json {
class Parser {
   public:
    Parser() = delete;
    Parser(const char *jsonString, bool isFile);
    ~Parser();

    Node *parse();

   private:
    bool isFile;
    char *source;
    Scanner *scanner;

    Node *parseTokens();
    Node *parseObjectTokens();
    Node *parseArrayTokens();

    Token peek();
    Token advance();

    char *readFile(const char *path);
};

}  // namespace Json

#endif
