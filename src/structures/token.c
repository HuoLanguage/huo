#include <stdbool.h>

#include "string.h"
#include "../base_util.h"

#define GENERATE_TOKEN_TYPE_ENUM_FUNCS 1
#include "token.h"
#undef GENERATE_TOKEN_TYPE_ENUM_FUNCS



bool char_is_digit(char c) {
    // By the C standard, 0-9 is guarinteed to be contigous.
    return c >= '0' && c <= '9';
}

static const struct String word_chars = STR_NEW("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
bool char_is_word(char c) {
    return string_contains(c, &word_chars);
}

bool can_be_extended(struct String so_far, char cur, enum token_type_t type) {
    switch (type) {
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_EX_MARK:
        case TOK_EQ:
        case TOK_LEFT_ANGLE:
        case TOK_RIGHT_ANGLE:
        case TOK_DOT:
        case TOK_COMMA:
        case TOK_OPEN_BRACKET:
        case TOK_CLOSE_BRACKET:
        case TOK_OPEN_SQUARE:
        case TOK_CLOSE_SQUARE:
            return false;
        case TOK_WHITESPACE:
        case TOK_NEWLINE:
        case TOK_NUMBER:
        case TOK_WORD:
            return can_be_of_type(cur, type);
        case TOK_QUOTE:
            // I.e. "not a newline" and "prev char, if not the starting quote, not a quote"
            return (so_far.length <= 1 || !can_be_of_type(so_far.body[so_far.length - 1], TOK_QUOTE)) && !can_be_of_type(cur, TOK_NEWLINE);
        case TOK_COMMENT:
            return !can_be_of_type(cur, TOK_NEWLINE);
        default:
            ERROR("Unknown type: '%d'", type);
    }
}

bool can_be_of_type(char c, enum token_type_t type) {
    if (c == 0) {
        return type == TOK_EOF;
    }
    switch (type) {
        case TOK_WHITESPACE:
            return c == ' ' || c == '\t';
        case TOK_NEWLINE:
            return c == '\n';
        case TOK_PLUS:
            return c == '+';
        case TOK_MINUS:
            return c == '-';
        case TOK_MUL:
            return c == '*';
        case TOK_DIV:
            return c == '/';
        case TOK_EX_MARK:
            return c == '!';
        case TOK_EQ:
            return c == '=';
        case TOK_LEFT_ANGLE:
            return c == '<';
        case TOK_RIGHT_ANGLE:
            return c == '>';
        case TOK_NUMBER:
            return char_is_digit(c);
        case TOK_DOT:
            return c == '.';
        case TOK_COMMA:
            return c == ',';
        case TOK_WORD:
            return char_is_word(c);
        case TOK_OPEN_BRACKET:
            return c == '(';
        case TOK_CLOSE_BRACKET:
            return c == ')';
        case TOK_OPEN_SQUARE:
            return c == '[';
        case TOK_CLOSE_SQUARE:
            return c == ']';
        case TOK_QUOTE:
            return c == '"';
        case TOK_COMMENT:
            return c == ';';
        default:
            ERROR("Unknown type: '%d'", type);
    }
}

enum token_type_t get_token_type(char c) {
    for (size_t i = 0; i < enum_len_token_type_t(); i++) {
        enum token_type_t type = enum_iter_token_type_t(i);
        if (can_be_of_type(c, type)) {
            return type;
        }
    }
    ERROR("Unknown character: '%c' = '%d'", c, c);
}
