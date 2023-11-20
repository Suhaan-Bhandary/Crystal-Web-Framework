#include "Scanner.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../Logger/Logger.h"
#include "../Token/Token.h"

using Token = Json::Token;
using TokenType = Json::TokenType;

Json::Scanner::Scanner(const char* rawJsonString, bool readFromFile) {
    // Initialize variables
    start = 0;
    current = 0;
    this->readFromFile = readFromFile;

    if (readFromFile) {
        source = readFile(rawJsonString);
    } else {
        source = (char*)rawJsonString;
    }
}

Json::Scanner::~Scanner() {
    // freeing the memory allocated by malloc if file was taken as input
    if (readFromFile) {
        free(source);
    }
}

std::vector<Token>& Json::Scanner::scanTokens() {
    // Initialize variables
    start = 0;
    current = 0;
    line = 1;

    while (!isAtEnd()) {
        start = current;

        // Check if scan was success or not
        bool isJsonTokensValid = scanToken();
        if (!isJsonTokensValid) {
            tokens.clear();
            return tokens;
        }
    }

    // Adding EOF
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", "", line));
    return tokens;
}

bool Json::Scanner::scanToken() {
    bool isScanComplete = true;

    char ch = advance();
    switch (ch) {
        case ',':
            addToken(TokenType::COMMA);
            break;

        case ':':
            addToken(TokenType::COLON);
            break;

        case '{':
            addToken(TokenType::OPEN_CURL_BRACKET);
            break;

        case '}':
            addToken(TokenType::CLOSE_CURL_BRACKET);
            break;

        case '[':
            addToken(TokenType::OPEN_SQUARE_BRACKET);
            break;

        case ']':
            addToken(TokenType::CLOSE_SQUARE_BRACKET);
            break;

        // we ignore all of this
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;

        // String
        case '"':
            isScanComplete = stringToken();
            break;

        default:
            if (isNumberStart(ch)) {
                isScanComplete = numberToken(ch);
            } else if (ch == '0' && peek() == '.') {
                isScanComplete = numberToken(ch);
            } else if (ch == '0' && (peek() < '1' || peek() > '9')) {
                std::string str(source + start, current - start);
                addToken(TokenType::NUMBER, (long long)0);
                isScanComplete = true;
            } else if (isAlpha(ch)) {
                isScanComplete = identifierToken();
            } else {
                LOGGER_ERROR("Unexpected character:", ch, " at line", line);
                isScanComplete = false;
            }
    }

    // No Error in token
    return isScanComplete;
}

bool Json::Scanner::stringToken() {
    while (!isAtEnd() && peek() != '"') {
        // Cntrl characters are not allowed
        if (std::iscntrl(static_cast<unsigned char>(peek()))) return false;

        if (peek() == '\\') {
            advance();

            std::unordered_set<char> escapeChars = {
                '"', '\\', '/', 'b', 'f', 'n', 'r', 't',
            };

            if (escapeChars.count(peek()) != 0) {
                advance();
            } else if (match('u') && isNextFourCharacterDigit()) {
                advance();

                advance();
                advance();
                advance();
                advance();
            } else {
                LOGGER_ERROR("Incorrect Escape Character", line);
                return false;
            }
        } else if (peek() == '\n') {
            LOGGER_ERROR("Unexpected end of string", line);
            return false;
        } else {
            advance();
        }
    }

    if (isAtEnd()) {
        LOGGER_ERROR("Unterminated String at line", line);
        return false;
    }

    // advance to "
    advance();

    std::string value(source + start + 1, (current - 1) - (start + 1));
    addToken(TokenType::STRING, value);

    return true;
}

bool Json::Scanner::numberToken(char firstChar) {
    if (firstChar == '-') {
        if (!(peek() >= '0' && peek() <= '9')) {
            LOGGER_ERROR(
                "Invalid Json Number: - should be followed by a digit at line",
                line);
            return false;
        }
    }

    // Take all numbers
    while (peek() >= '0' && peek() <= '9') {
        advance();
    }

    // check if it is a . or e
    if (match('.')) {
        bool isDigitAfterDot = false;

        // take all numbers
        while (peek() >= '0' && peek() <= '9') {
            isDigitAfterDot = true;
            advance();
        }

        if (!isDigitAfterDot) {
            LOGGER_ERROR(
                "Invalid Json Number, . should have digit after it at line",
                line);
            return false;
        }

        if (match('e') || match('E')) {
            if (peek() == '+' || peek() == '-') {
                advance();
            }

            bool isDigitAfterE = false;
            while (peek() >= '0' && peek() <= '9') {
                isDigitAfterE = true;
                advance();
            }

            if (!isDigitAfterE) {
                LOGGER_ERROR(
                    "Invalid Json Number, e/E should have digit after it at "
                    "line",
                    line);
                return false;
            }
        }

        std::string str(source + start, current - start);
        double value = stod(str);
        addToken(TokenType::FRACTION, value);
        return true;
    } else if (match('e') || match('E')) {
        if (peek() == '+' || peek() == '-') {
            advance();
        }

        bool isDigitAfterE = false;
        while (peek() >= '0' && peek() <= '9') {
            isDigitAfterE = true;
            advance();
        }

        if (!isDigitAfterE) {
            LOGGER_ERROR(
                "Invalid Json Number, e/E should have digit after it at "
                "line",
                line);
            return false;
        }

        std::string str(source + start, current - start);
        double value = stod(str);
        addToken(TokenType::FRACTION, value);
        return true;
    } else {
        // Non Fraction
        std::string str(source + start, current - start);
        long long value = stoll(str);
        addToken(TokenType::NUMBER, value);
        return true;
    }
}

bool Json::Scanner::identifierToken() {
    while (!isAtEnd() && isAlpha(peek())) {
        advance();
    }

    std::unordered_map<std::string, TokenType> keywords = {
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"null", TokenType::NULL_TOKEN}};

    // Check if keyword is present or not
    std::string token(source + start, current - start);

    if (keywords.find(token) == keywords.end()) {
        LOGGER_ERROR("Invalid Keyword", token, "at line", line);
        return false;
    }

    TokenType type = keywords[token];
    addToken(type);

    return true;
}

void Json::Scanner::addToken(TokenType type) {
    std::string lexeme(source + start, current - start);
    tokens.push_back(Token(type, lexeme, line));
}

void Json::Scanner::addToken(TokenType type, std::string literal) {
    std::string lexeme(source + start, current - start);
    tokens.push_back(Token(type, lexeme, literal, line));
}

void Json::Scanner::addToken(TokenType type, double literal) {
    std::string lexeme(source + start, current - start);
    tokens.push_back(Token(type, lexeme, literal, line));
}

void Json::Scanner::addToken(TokenType type, long long literal) {
    std::string lexeme(source + start, current - start);
    tokens.push_back(Token(type, lexeme, literal, line));
}

char Json::Scanner::advance() { return source[current++]; }

bool Json::Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Json::Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

bool Json::Scanner::isAtEnd() { return source[current] == '\0'; }

bool Json::Scanner::isNextFourCharacterDigit() {
    if (!isHexCharacter(source[current + 0])) return false;
    if (!isHexCharacter(source[current + 1])) return false;
    if (!isHexCharacter(source[current + 2])) return false;
    if (!isHexCharacter(source[current + 3])) return false;
    return true;
}

bool Json::Scanner::isHexCharacter(char ch) {
    return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') ||
            (ch >= 'a' && ch <= 'f'));
}

bool Json::Scanner::isNumberStart(char ch) {
    return (ch == '-' || (ch >= '1' && ch <= '9'));
}

bool Json::Scanner::isAlpha(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
}

char* Json::Scanner::readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}
