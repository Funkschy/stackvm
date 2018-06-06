#include <stdio.h>
#include <string.h>

#include "../cli/common.h"
#include "scanner.h"

void init_scanner(Scanner *scanner, const char *source) {
    scanner->start = source;
    scanner->current = source;
}

static bool at_end(Scanner *scanner) {
    return *scanner->current == '\0';
}

static char advance(Scanner *scanner) {
    scanner->current++;
    return scanner->current[-1];
}

static inline char peek(Scanner *scanner) {
    return *scanner->current;
}

static inline char peek_next(Scanner *scanner) {
    if (at_end(scanner)) return '\0';
    return scanner->current[1];
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static Token make_token(Scanner *scanner, TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = (unsigned int) (scanner->current - scanner->start);

    return token;
}

static Token number(Scanner *scanner) {
    while (is_digit(peek(scanner))) advance(scanner);

    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
        advance(scanner);
        while (is_digit(peek(scanner))) advance(scanner);
    }

    return make_token(scanner, TOKEN_NUMBER);
}

static void skip_whitespace(Scanner *scanner) {
    while (true) {
        char c = *scanner->current;

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                advance(scanner);
                break;
            case '/':
                if(peek_next(scanner) == '/') {
                    while (!at_end(scanner) && peek(scanner) != '\n') {
                        advance(scanner);
                    }
                }
                break;
            default:
                return;
        }
    }
}

static Token error_token(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);

    return token;
}

static TokenType check_keyword(Scanner *scanner, int start, size_t rest_length, const char *rest, TokenType type) {
    if (scanner->current - scanner->start == start + rest_length
        && memcmp(scanner->start + start, rest, rest_length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Scanner *scanner) {
    switch (*scanner->start) {
        case 'v':
            return check_keyword(scanner, 1, 2, "ar", TOKEN_VAR);
        case 'p':
            return check_keyword(scanner, 1, 4, "rint", TOKEN_PRINT);
        case 'w':
            return check_keyword(scanner, 1, 4, "hile", TOKEN_WHILE);
        case 'i':
            return check_keyword(scanner, 1, 1, "f", TOKEN_IF);
        case 'e':
            return check_keyword(scanner, 1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner->current - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 'u':
                        return check_keyword(scanner, 2, 1, "n", TOKEN_FUN);
                    case 'a':
                        return check_keyword(scanner, 2, 3, "lse", TOKEN_FALSE);
                }
            }
        case 't':
            return check_keyword(scanner, 1, 3, "rue", TOKEN_TRUE);
        case 'r':
            return check_keyword(scanner, 1, 5, "eturn", TOKEN_RETURN);
        default:
            return TOKEN_IDENTIFIER;
    }
}

static Token identifier(Scanner *scanner) {
    while (is_alpha(peek(scanner)) || is_digit(peek(scanner))) advance(scanner);
    return make_token(scanner, identifier_type(scanner));
}

static Token string(Scanner *scanner) {
    while (!at_end(scanner) && peek(scanner) != '"')
        advance(scanner);

    if (at_end(scanner))
        return error_token("Unterminated String");

    advance(scanner);
    return make_token(scanner, TOKEN_STRING);
}

Token scan_token(Scanner *scanner) {
    skip_whitespace(scanner);

    scanner->start = scanner->current;

    if (at_end(scanner)) return make_token(scanner, TOKEN_EOF);

    char c = advance(scanner);

    if (is_digit(c)) return number(scanner);
    if (is_alpha(c)) return identifier(scanner);

    switch (c) {
        case '(':
            return make_token(scanner, TOKEN_OPEN_PAREN);
        case ')':
            return make_token(scanner, TOKEN_CLOSE_PAREN);
        case '+':
            return make_token(scanner, TOKEN_PLUS);
        case '-':
            if (peek(scanner) == '>') {
                advance(scanner);
                return make_token(scanner, TOKEN_ARROW);
            } else {
                return make_token(scanner, TOKEN_MINUS);
            }
        case '*':
            return make_token(scanner, TOKEN_STAR);
        case '%':
            return make_token(scanner, TOKEN_PERCENT);
        case '/':
            return make_token(scanner, TOKEN_SLASH);
        case '{':
            return make_token(scanner, TOKEN_OPEN_BRACE);
        case '}':
            return make_token(scanner, TOKEN_CLOSE_BRACE);
        case ',':
            return make_token(scanner, TOKEN_COMMA);
        case '=':
            if (peek(scanner) == '=') {
                advance(scanner);
                return make_token(scanner, TOKEN_EQUALS_EQUALS);
            } else {
                return make_token(scanner, TOKEN_EQUALS);
            }
        case '!':
            if (peek(scanner) == '=') {
                advance(scanner);
                return make_token(scanner, TOKEN_BANG_EQUALS);
            } else {
                return make_token(scanner, TOKEN_BANG);
            }
        case ';':
            return make_token(scanner, TOKEN_SEMICOLON);
        case '"':
            return string(scanner);
        default:
            return error_token("Unexpected Character");
    }
}