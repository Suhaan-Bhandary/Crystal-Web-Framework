#include "Parser.h"

#include "../../Logger/Logger.h"
#include "../Scanner/Scanner.h"
#include "../Token/Token.h"

using Token = Json::Token;
using Scanner = Json::Scanner;

Json::Parser::Parser(const char *jsonString, bool isFile) {
    // Initialize variables
    this->isFile = isFile;

    if (isFile) {
        source = readFile(jsonString);
    } else {
        source = (char *)jsonString;
    }

    // creating the scanner
    scanner = new Scanner(source);
}

Json::Parser::~Parser() {
    delete scanner;
    // freeing the memory allocated by malloc if file was taken as input
    if (isFile) {
        free(source);
    }
}

// Utility Function to display string for a token
inline std::string tokenToString(Json::Token &token) {
    switch (token.type) {
        case Json::TokenType::COMMA:
            return "COMMA";
        case Json::TokenType::COLON:
            return "COLON";
        case Json::TokenType::OPEN_CURL_BRACKET:
            return "OPEN_CURL_BRACKET";
        case Json::TokenType::CLOSE_CURL_BRACKET:
            return "CLOSE_CURL_BRACKET";
        case Json::TokenType::OPEN_SQUARE_BRACKET:
            return "OPEN_SQUARE_BRACKET";
        case Json::TokenType::CLOSE_SQUARE_BRACKET:
            return "CLOSE_SQUARE_BRACKET";
        case Json::TokenType::STRING:
            return "STRING " + std::get<Json::String>(token.literal);
        case Json::TokenType::NUMBER:
            return "NUMBER " +
                   std::to_string(std::get<Json::Number>(token.literal));
        case Json::TokenType::UNSIGNED_NUMBER:
            return "UNSIGNED_NUMBER " +
                   std::to_string(std::get<Json::UnsignedNumber>(token.literal));
        case Json::TokenType::FRACTION:
            return "FRACTION " +
                   std::to_string(std::get<Json::Fraction>(token.literal));
        case Json::TokenType::TRUE:
            return "TRUE";
        case Json::TokenType::FALSE:
            return "FALSE";
        case Json::TokenType::NULL_TOKEN:
            return "NULL_TOKEN";
        case Json::TokenType::EOF_TOKEN:
            return "EOF_TOKEN";
        default:
            return "";
    }
}

Json::Node *Json::Parser::parse() {
    // If tokens is empty then an error has occurred
    Token firstToken = peek();
    if (firstToken.type == INVALID) {
        return new Node();
    }

    // Official: A JSON payload should be an object or array, not a string
    // So first token should be { or [
    if (firstToken.type != TokenType::OPEN_CURL_BRACKET &&
        firstToken.type != TokenType::OPEN_SQUARE_BRACKET) {
        return new Node();
    }

    // Parse Token
    Node *node = parseTokens();

    if (node == nullptr) {
        return new Node();
    }

    // Check if it is at EOF or not
    if (peek().type != TokenType::EOF_TOKEN) {
        LOGGER_ERROR("Expected EOF at line ", peek().line);
        return new Node();
    }

    return node;
}

Json::Node *Json::Parser::parseTokens() {
    auto token = advance();

    switch (token.type) {
        case TokenType::OPEN_CURL_BRACKET:
            return parseObjectTokens();
        case TokenType::OPEN_SQUARE_BRACKET:
            return parseArrayTokens();
        case TokenType::STRING: {
            return new Node(std::get<String>(token.literal));
        }
        case TokenType::NUMBER: {
            return new Node(std::get<Number>(token.literal));
        }
        case TokenType::UNSIGNED_NUMBER: {
            return new Node(std::get<UnsignedNumber>(token.literal));
        }
        case TokenType::FRACTION: {
            return new Node(std::get<Fraction>(token.literal));
        }
        case TokenType::TRUE: {
            return new Node(true);
        }
        case TokenType::FALSE: {
            return new Node(false);
        }
        case TokenType::NULL_TOKEN: {
            return new Node();
        }
        default:
            std::string lexeme(source + token.lexemeStart, token.lexemeLength);
            LOGGER_ERROR("Invalid Token Found", lexeme, "at line", token.line);
            return nullptr;
    }
}

Json::Node *Json::Parser::parseObjectTokens() {
    // If the first token is } then it is an empty object
    if (peek().type == CLOSE_CURL_BRACKET) {
        advance();
        return new Node(Object());
    }

    Node *node = new Node(Object());

    // parse key value { key : value }
    while (true) {
        // key
        auto keyToken = advance();
        if (keyToken.type != TokenType::STRING) {
            // clean the node
            delete node;
            std::string lexeme(source + keyToken.lexemeStart,
                               keyToken.lexemeLength);
            LOGGER_ERROR("Json Object Error: key should be string at line",
                         keyToken.line, "but found", lexeme);
            return nullptr;
        }

        // Store key in variable
        String key = std::get<String>(keyToken.literal);

        // colon
        auto colonToken = advance();
        if (colonToken.type != TokenType::COLON) {
            // clean the node
            delete node;
            std::string lexeme(source + colonToken.lexemeStart,
                               colonToken.lexemeLength);
            LOGGER_ERROR("Json Object Error: Expected colon separator at line",
                         colonToken.line, "but found", lexeme);
            return nullptr;
        }

        // value
        Node *valueNode = parseTokens();
        if (valueNode == nullptr) {
            // clean the node
            delete node;
            return nullptr;
        }

        // insert key and value
        node->set(key, valueNode);

        auto separator = advance();
        if (separator.type == TokenType::COMMA) {
            continue;
        } else if (separator.type == TokenType::CLOSE_CURL_BRACKET) {
            break;
        } else {
            // clean the node
            delete node;
            std::string lexeme(source + separator.lexemeStart,
                               separator.lexemeLength);
            LOGGER_ERROR("Json Object Error: Expected comma or color at line",
                         separator.line, "but found", lexeme);
            return nullptr;
        }
    }

    return node;
}

Json::Node *Json::Parser::parseArrayTokens() {
    // If the first token is } then it is an empty object
    if (peek().type == CLOSE_SQUARE_BRACKET) {
        advance();
        return new Node(Array());
    }

    Node *node = new Node(Array());

    // parse as: value, value ]
    while (true) {
        // value
        auto token = peek();
        Node *valueNode = parseTokens();
        if (valueNode == nullptr) {
            // clean the node
            delete node;
            return nullptr;
        }

        // push value in array
        node->push(valueNode);

        auto separator = advance();
        if (separator.type == TokenType::COMMA) {
            continue;
        } else if (separator.type == TokenType::CLOSE_SQUARE_BRACKET) {
            break;
        } else {
            // clean the node
            delete node;
            std::string lexeme(source + separator.lexemeStart,
                               separator.lexemeLength);
            LOGGER_ERROR(
                "Json Array Error: Expected colon or comma separator at line",
                separator.line, "but found", lexeme);

            return nullptr;
        }
    }

    return node;
}

// Util functions
Token Json::Parser::peek() {
    return scanner->peekToken();
}

// advance if token present, else Returning EOF if is at end
Token Json::Parser::advance() {
    return scanner->advanceToken();
}

char *Json::Parser::readFile(const char *path) {
    FILE *file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}
