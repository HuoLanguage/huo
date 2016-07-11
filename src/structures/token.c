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
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case EX_MARK:
        case EQ:
        case LEFT_ANGLE:
        case RIGHT_ANGLE:
        case DOT:
        case COMMA:
        case OPEN_BRACKET:
        case CLOSE_BRACKET:
        case OPEN_SQUARE:
        case CLOSE_SQUARE:
            return false;
        case WHITESPACE:
        case NEWLINE:
        case NUMBER:
        case WORD:
            return can_be_of_type(cur, type);
        case QUOTE:
            // I.e. "not a newline" and "prev char, if not the starting quote, not a quote"
            return (so_far.length <= 1 || !can_be_of_type(so_far.body[so_far.length - 1], QUOTE)) && !can_be_of_type(cur, NEWLINE);
        case COMMENT:
            return !can_be_of_type(cur, NEWLINE);
        default:
            ERROR("Unknown type: '%d'", type);
    }
}

bool can_be_of_type(char c, enum token_type_t type) {
    if (c == 0) {
        return type == T_EOF;
    }
    switch (type) {
        case WHITESPACE:
            return c == ' ' || c == '\t';
        case NEWLINE:
            return c == '\n';
        case PLUS:
            return c == '+';
        case MINUS:
            return c == '-';
        case MUL:
            return c == '*';
        case DIV:
            return c == '/';
        case EX_MARK:
            return c == '!';
        case EQ:
            return c == '=';
        case LEFT_ANGLE:
            return c == '<';
        case RIGHT_ANGLE:
            return c == '>';
        case NUMBER:
            return char_is_digit(c);
        case DOT:
            return c == '.';
        case COMMA:
            return c == ',';
        case WORD:
            return char_is_word(c);
        case OPEN_BRACKET:
            return c == '(';
        case CLOSE_BRACKET:
            return c == ')';
        case OPEN_SQUARE:
            return c == '[';
        case CLOSE_SQUARE:
            return c == ']';
        case QUOTE:
            return c == '"';
        case COMMENT:
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
