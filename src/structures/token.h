#include <stdint.h>
#include <stdlib.h>

// Here be dragons.

// Basic idea:
// Header includes itself 3x if GENERATE_TOKEN_TYPE_ENUM_FUNCS
// Once to do the enum definition
// Once to do the enum->string mapping
// Once to do the enum iterator.

#if GENERATE_TOKEN_TYPE_ENUM_FUNCS
#if GENERATED_TOKEN_TYPE_ENUM_FUNCS
#error "Generating token type enum functions in multiple places!"
#endif

#undef GENERATE_TOKEN_TYPE_ENUM_FUNCS
#include "token.h"

#define GENERATE_ENUM_STRINGS 1
#include "token.h"
#undef GENERATE_ENUM_STRINGS

#define GENERATE_ENUM_ITER 1
#include "token.h"
#undef GENERATE_ENUM_ITER

#define GENERATED_TOKEN_TYPE_ENUM_FUNCS 1
#endif

#undef USE_HEADER_GUARD
#if GENERATE_ENUM_STRINGS
    #define DECL_ENUM_ELEMENT(ELEMENT) #ELEMENT
    #define BEGIN_ENUM(ENUM_NAME) static const char* enum_names_##ENUM_NAME [] =
    #define END_ENUM(ENUM_NAME) ; \
    const char* enum_name_##ENUM_NAME(enum ENUM_NAME index){\
        return enum_names_##ENUM_NAME [index];\
    }
#elif GENERATE_ENUM_ITER
    #define DECL_ENUM_ELEMENT(ELEMENT) ELEMENT
    #define BEGIN_ENUM(ENUM_NAME) static const enum ENUM_NAME enum_iters_##ENUM_NAME [] =
    #define END_ENUM(ENUM_NAME) ; \
    enum ENUM_NAME enum_iter_##ENUM_NAME(enum ENUM_NAME index){\
        return enum_iters_##ENUM_NAME [index];\
    } \
    size_t enum_len_##ENUM_NAME() {return sizeof(enum_iters_##ENUM_NAME) / sizeof(enum_iters_##ENUM_NAME[0]);}
#else
    #define USE_HEADER_GUARD 1
    #define DECL_ENUM_ELEMENT(ELEMENT) ELEMENT
    #define BEGIN_ENUM(ENUM_NAME) enum ENUM_NAME
    #define END_ENUM(ENUM_NAME) ; \
            const char* enum_name_##ENUM_NAME(enum ENUM_NAME index); \
            enum ENUM_NAME enum_iter_##ENUM_NAME(enum ENUM_NAME index); \
            size_t enum_len_##ENUM_NAME();
#endif

#if !USE_HEADER_GUARD || !TOKEN_TYPE_DEC

#if USE_HEADER_GUARD
#define TOKEN_TYPE_DEC 1
#endif

BEGIN_ENUM(token_type_t) {
    DECL_ENUM_ELEMENT(TOK_WHITESPACE), // " \t"
    DECL_ENUM_ELEMENT(TOK_NEWLINE), // "\n"
    DECL_ENUM_ELEMENT(TOK_PLUS), // "+"
    DECL_ENUM_ELEMENT(TOK_MINUS), // "-"
    DECL_ENUM_ELEMENT(TOK_BUILTIN), // [*\!=<>&|]
    DECL_ENUM_ELEMENT(TOK_NUMBER), // [0-9]+
    DECL_ENUM_ELEMENT(TOK_DOT), // "."
    DECL_ENUM_ELEMENT(TOK_COMMA), // ","
    DECL_ENUM_ELEMENT(TOK_WORD), // [_a-zA-Z]+
    DECL_ENUM_ELEMENT(TOK_OPEN_BRACKET), // "("
    DECL_ENUM_ELEMENT(TOK_CLOSE_BRACKET), // ")"
    DECL_ENUM_ELEMENT(TOK_OPEN_SQUARE), // "["
    DECL_ENUM_ELEMENT(TOK_CLOSE_SQUARE), // "]"
    DECL_ENUM_ELEMENT(TOK_QUOTE), // """
    DECL_ENUM_ELEMENT(TOK_COMMENT), // ";"
    DECL_ENUM_ELEMENT(TOK_EOF) // end of file
} END_ENUM(token_type_t)

#endif

#undef DECL_ENUM_ELEMENT
#undef BEGIN_ENUM
#undef END_ENUM

#ifndef _TOKEN_H
#define _TOKEN_H 1

struct Token {
    enum token_type_t type;
    struct String data;
};

struct Tokens {
    struct Token *tokens;
    size_t length;
    size_t counter;
};

bool can_be_extended(struct String so_far, char cur, enum token_type_t type);

bool can_be_of_type(char c, enum token_type_t type);

enum token_type_t get_token_type(char c);

#endif
