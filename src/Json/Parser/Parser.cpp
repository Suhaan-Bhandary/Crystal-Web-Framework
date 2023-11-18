#include "Parser.h"

#include <iostream>
#include <memory>

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

    // Tokens for Debugging
    // std::cout << std::endl;
    // int line = 0;
    // for (auto &token : tokens) {
    //     if (line != token.line) {
    //         line = token.line;
    //         std::cout << "Line: " << token.line << ": " << std::endl;
    //     }

    //     std::cout << "\t" << tokenToString(token) << std::endl;
    // }
    // std::cout << std::endl;

    // If tokens is empty then an error has occurred
    if (tokens.empty()) {
        std::cout << "Error while Scanning Json" << std::endl;
        return new Node();
    }

    // Parse Token
    Node *node = parseTokens();

    // Check the last token
    auto token = advance();

    if (token.type != TokenType::EOF_TOKEN) {
        std::cout << "Expected EOF at line " << token.line << std::endl;
        return new Node();
    }

    if (node == nullptr) {
        std::cout << "Error while Parsing Json" << std::endl;
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
            std::cout << "Incorrect Token: " << token.line << ", "
                      << token.lexeme << std::endl;
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
            std::cout << "Json Object Error: " << keyToken.line << ", "
                      << keyToken.lexeme << std::endl;
            return nullptr;
        }

        // Store key in variable
        String key = std::get<String>(keyToken.literal);

        // colon
        auto colonToken = advance();
        if (colonToken.type != TokenType::COLON) {
            // clean the node
            delete node;
            std::cout << "Json Object Error: " << colonToken.line << ", "
                      << colonToken.lexeme << std::endl;
            return nullptr;
        }

        // value
        Node *valueNode = parseTokens();
        if (valueNode == nullptr) {
            // clean the node
            delete node;
            std::cout << "Json Object Error: " << colonToken.line << ", "
                      << colonToken.lexeme << std::endl;
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
            std::cout << "Json Object Error: " << colonToken.line << ", "
                      << colonToken.lexeme << std::endl;
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
            std::cout << "Json Array Error at " << token.line << std::endl;
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
            std::cout << "Json Array Error at " << token.line << std::endl;
            return nullptr;
        }
    }

    return node;
}

// Util functions
Token &Json::Parser::peek() { return tokens[current]; }
Token &Json::Parser::advance() { return tokens[current++]; }
bool Json::Parser::isAtEnd() { return tokens[current].type == EOF_TOKEN; }