#include "Scanner.h"

#include "../../Logger/Logger.h"
#include "../Token/Token.h"

using Token = Crystal::Json::Token;
using TokenType = Crystal::Json::TokenType;

Crystal::Json::Scanner::Scanner(const char* source) {
    // Initialize variables
    start = 0;
    current = 0;
    line = 1;
    this->source = source;
}

Crystal::Json::Scanner::Scanner(const Scanner& otherScanner) {
    start = otherScanner.start;
    current = otherScanner.current;
    line = otherScanner.line;
    source = otherScanner.source;
}

Crystal::Json::Scanner& Crystal::Json::Scanner::operator=(
    const Scanner& otherScanner) {
    if (this != &otherScanner) {
        start = otherScanner.start;
        current = otherScanner.current;
        line = otherScanner.line;
        source = otherScanner.source;
    }
    return *this;
}

Crystal::Json::Scanner::Scanner(Scanner&& otherScanner) {
    start = otherScanner.start;
    current = otherScanner.current;
    line = otherScanner.line;
    source = otherScanner.source;
}

Crystal::Json::Scanner& Crystal::Json::Scanner::operator=(
    Scanner&& otherScanner) {
    start = otherScanner.start;
    current = otherScanner.current;
    line = otherScanner.line;
    source = otherScanner.source;
    return *this;
}

Token Crystal::Json::Scanner::scanToken() {
    // set start as current
    start = current;

    // skipping all the unnecessary tokens
    trimCharacters();

    // Return EOF if it is at end
    if (isAtEnd()) {
        return Token(TokenType::EOF_TOKEN, start, current - start, line);
    }

    char ch = advance();
    switch (ch) {
        case ',':
            return createToken(TokenType::COMMA);

        case ':':
            return createToken(TokenType::COLON);

        case '{':
            return createToken(TokenType::OPEN_CURL_BRACKET);

        case '}':
            return createToken(TokenType::CLOSE_CURL_BRACKET);

        case '[':
            return createToken(TokenType::OPEN_SQUARE_BRACKET);

        case ']':
            return createToken(TokenType::CLOSE_SQUARE_BRACKET);

        // String
        case '"':
            return stringToken();

        default:
            if (isNumberStart(ch)) {
                return numberToken(ch);
            } else if (ch == '0' && peek() == '.') {
                return numberToken(ch);
            } else if (ch == '0' && (peek() < '1' || peek() > '9')) {
                return createToken(TokenType::NUMBER, (long long)0);
            } else if (isAlpha(ch)) {
                return identifierToken();
            } else {
                LOGGER_ERROR("Unexpected character:", ch, " at line", line);
                return createToken(TokenType::INVALID);
            }
    }
}

Token Crystal::Json::Scanner::stringToken() {
    while (!isAtEnd() && peek() != '"') {
        // Cntrl characters are not allowed
        if (std::iscntrl(static_cast<unsigned char>(peek())))
            return createToken(TokenType::INVALID);

        if (match('\\')) {
            char ch = peek();
            if (ch == '\\' || ch == '"' || ch == '/' || ch == 'b' ||
                ch == 'f' || ch == 'n' || ch == 'r' || ch == 't') {
                advance();
            } else if (match('u') && isNextFourCharacterDigit()) {
                advance();

                advance();
                advance();
                advance();
                advance();
            } else {
                LOGGER_ERROR("Incorrect Escape Character", line);
                return createToken(TokenType::INVALID);
            }
        } else if (peek() == '\n') {
            LOGGER_ERROR("Unexpected end of string", line);
            return createToken(TokenType::INVALID);
        } else {
            advance();
        }
    }

    if (isAtEnd()) {
        LOGGER_ERROR("Unterminated String at line", line);
        return createToken(TokenType::INVALID);
    }

    // advance to "
    advance();

    std::string value(source + start + 1, (current - 1) - (start + 1));
    return createToken(TokenType::STRING, value);
}

Token Crystal::Json::Scanner::numberToken(char firstChar) {
    if (firstChar == '-') {
        if (!(peek() >= '0' && peek() <= '9')) {
            LOGGER_ERROR(
                "Invalid Json Number: - should be followed by a digit at line",
                line);
            return createToken(TokenType::INVALID);
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
            return createToken(TokenType::INVALID);
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
                return createToken(TokenType::INVALID);
            }
        }

        std::string str(source + start, current - start);
        double value = stod(str);
        return createToken(TokenType::FRACTION, value);
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
            return createToken(TokenType::INVALID);
        }

        std::string str(source + start, current - start);
        double value = stod(str);
        return createToken(TokenType::FRACTION, value);
    } else {
        // Non Fraction
        std::string str(source + start, current - start);

        // Handle out of range, if out-of range then we keep it as number only
        try {
            long long value = stoll(str);
            return createToken(TokenType::NUMBER, value);
        } catch (const std::out_of_range& e) {
            // store it in stoull if it cannot be stored in long long, else we
            // make it null
            try {
                unsigned long long value = stoull(str);
                return createToken(TokenType::UNSIGNED_NUMBER, value);
            } catch (const std::out_of_range& e) {
                return createToken(TokenType::NULL_TOKEN);
            }
        }
    }
}

Token Crystal::Json::Scanner::identifierToken() {
    while (!isAtEnd() && isAlpha(peek())) {
        advance();
    }

    // Check if keyword is present or not
    std::string token(source + start, current - start);

    // Match the token and assign valid type
    TokenType type = TokenType::INVALID;
    if (token == "true") {
        type = TokenType::TRUE;
    } else if (token == "false") {
        type = TokenType::FALSE;
    } else if (token == "null") {
        type = TokenType::NULL_TOKEN;
    } else {
        LOGGER_ERROR("Invalid Keyword", token, "at line", line);
    }

    return createToken(type);
}

Token Crystal::Json::Scanner::createToken(TokenType type) {
    return Token(type, start, current - start, line);
}

Token Crystal::Json::Scanner::createToken(TokenType type,
                                          const std::string& literal) {
    return Token(type, start, current - start, literal, line);
}

Token Crystal::Json::Scanner::createToken(TokenType type, double literal) {
    return Token(type, start, current - start, literal, line);
}

Token Crystal::Json::Scanner::createToken(TokenType type, long long literal) {
    return Token(type, start, current - start, literal, line);
}

Token Crystal::Json::Scanner::createToken(TokenType type,
                                          unsigned long long literal) {
    return Token(type, start, current - start, literal, line);
}

char Crystal::Json::Scanner::advance() { return source[current++]; }

bool Crystal::Json::Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Crystal::Json::Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

// Public functions for Parser
bool Crystal::Json::Scanner::isAtEnd() { return source[current] == '\0'; }

Token Crystal::Json::Scanner::advanceToken() { return scanToken(); }

Token Crystal::Json::Scanner::peekToken() {
    // store variables before advance
    int initialStart = start;
    int initialCurrent = current;
    int initialLine = line;

    // We advance one token and store the result and then reset the state
    Token token = scanToken();

    // reset variables
    start = initialStart;
    current = initialCurrent;
    line = initialLine;

    return token;
}

void Crystal::Json::Scanner::trimCharacters() {
    // we ignore all of this values
    while (source[current] == ' ' || source[current] == '\r' ||
           source[current] == '\t' || source[current] == '\n') {
        if (source[current] == '\n') line++;
        current++;
    }

    // set the start as current after the trim
    start = current;
}

bool Crystal::Json::Scanner::isNextFourCharacterDigit() {
    if (!isHexCharacter(source[current + 0])) return false;
    if (!isHexCharacter(source[current + 1])) return false;
    if (!isHexCharacter(source[current + 2])) return false;
    if (!isHexCharacter(source[current + 3])) return false;
    return true;
}

bool Crystal::Json::Scanner::isHexCharacter(char ch) {
    return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') ||
            (ch >= 'a' && ch <= 'f'));
}

bool Crystal::Json::Scanner::isNumberStart(char ch) {
    return (ch == '-' || (ch >= '1' && ch <= '9'));
}

bool Crystal::Json::Scanner::isAlpha(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
}
