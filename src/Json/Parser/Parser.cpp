#include "Parser.h"

#include <iostream>
#include <memory>

#include "../../Logger/Logger.h"
#include "../Scanner/Scanner.h"
#include "../Token/Token.h"

using Token = Json::Token;
using Scanner = Json::Scanner;

Json::Parser::Parser(const char *jsonString, bool isFile) {
    start = 0;
    current = 0;
    source = jsonString;
    readFromFile = isFile;
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
            return "STRING " + token.lexeme;
        case Json::TokenType::NUMBER:
            return "NUMBER " + token.lexeme;
        case Json::TokenType::FRACTION:
            return "FRACTION " + token.lexeme;
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
    start = 0;
    current = 0;
    tokens.clear();

    // Scanner the Json and get Tokens
    Scanner scanner(source, readFromFile);
    tokens = scanner.scanTokens();

    // LOGGER("");
    // LOGGER_MINIMAL("Debugging Scanner Tokens");
    // int line = 0;
    // for (auto &token : tokens) {
    //     if (line != token.line) {
    //         line = token.line;
    //         LOGGER_MINIMAL("Line " + std::to_string(token.line) + ": ");
    //     }

    //     LOGGER_MINIMAL("\t" + tokenToString(token));
    // }
    // LOGGER_MINIMAL("");

    // If tokens is empty then an error has occurred
    if (tokens.empty()) {
        return new Node();
    }

    // Parse Token
    Node *node = parseTokens();

    // Check the last token
    auto token = advance();

    if (node == nullptr) {
        return new Node();
    }

    if (token.type != TokenType::EOF_TOKEN) {
        LOGGER_ERROR("Expected EOF at line ", token.line);
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
            LOGGER_ERROR("Invalid Token Found ", token.line);
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
            LOGGER_ERROR("Json Object Error: key should be string at line",
                         keyToken.line, "but found", keyToken.lexeme);
            return nullptr;
        }

        // Store key in variable
        String key = std::get<String>(keyToken.literal);

        // colon
        auto colonToken = advance();
        if (colonToken.type != TokenType::COLON) {
            // clean the node
            delete node;
            LOGGER_ERROR("Json Object Error: Expected colon separator at line",
                         colonToken.line, "but found", colonToken.lexeme);
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
            LOGGER_ERROR("Json Object Error: Expected comma or color at line",
                         separator.line, "but found", separator.lexeme);
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
            LOGGER_ERROR(
                "Json Array Error: Expected colon or comma separator at line",
                separator.line, "but found", separator.lexeme);

            return nullptr;
        }
    }

    return node;
}

// Util functions
Token &Json::Parser::peek() { return tokens[current]; }
Token &Json::Parser::advance() { return tokens[current++]; }
bool Json::Parser::isAtEnd() { return tokens[current].type == EOF_TOKEN; }