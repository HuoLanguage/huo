#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "structures/structures.h"
#include "constants.h"
#include "core_functions.h"
#include "config.h"

typedef huo_ast *parse_func(size_t *pos, struct Tokens *tokens);

void free_if_nonnull(void *v);

struct Token get_token(size_t *pos, struct Tokens *tokens);
bool accept(huo_ast **, size_t *pos, struct Tokens *tokens, parse_func *to_accept);
bool discard(size_t *pos, struct Tokens *tokens, parse_func *to_accept);
bool discard_any(size_t *pos, struct Tokens *tokens, size_t num_to_accept, parse_func **to_accept);
bool accept_any(huo_ast **ret, size_t *pos, struct Tokens *tokens, size_t num_to_accept, parse_func **to_accept);
huo_ast *parse_whitespace(size_t *pos, struct Tokens *tokens);
huo_ast *parse_newline(size_t *pos, struct Tokens *tokens);
huo_ast *parse_comment(size_t *pos, struct Tokens *tokens);
bool discard_whitespace(size_t *pos, struct Tokens *tokens);
huo_ast *parse_eof(size_t *pos, struct Tokens *tokens);
huo_ast *parse_main(size_t *pos, struct Tokens *tokens);
huo_ast *parse_function(size_t *pos, struct Tokens *tokens);
huo_ast *parse_open_bracket_internals(size_t *pos, struct Tokens *tokens);
huo_ast *parse_open_bracket(size_t *pos, struct Tokens *tokens);
huo_ast *parse_statement(size_t *pos, struct Tokens *tokens);
huo_ast *parse_float(size_t *pos, struct Tokens *tokens);
huo_ast *parse_int(size_t *pos, struct Tokens *tokens);
huo_ast *parse_number(size_t *pos, struct Tokens *tokens);
huo_ast *parse_word(size_t *pos, struct Tokens *tokens);
huo_ast *parse_quote(size_t *pos, struct Tokens *tokens);
huo_ast *parse_open_square(size_t *pos, struct Tokens *tokens);

#define EXPECT(pos, tokens, to, x) do {if (!accept((to), (pos), (tokens), (x))) {return NULL;}} while (0)
#define EXPECT_TOKEN(pos, tokens, x) do {if (get_token((pos), (tokens)).type != (x)) {return NULL;} (*(pos)) += 1;} while (0)

char *token_print(struct Token t) {
    struct String st = string_from_chars(enum_name_token_type_t(t.type));
    struct String *s = string_copy_heap(&st);
    struct String c = string_from_chars(": ");
    string_concat_to(s, &c);
    string_concat_to(s, &t.data);
    return string_to_chars(s);
}

void free_if_nonnull(void *v) {
   if (v != NULL)
       free(v);
}

struct Token get_token(size_t *pos, struct Tokens *tokens) {
    //printf("%zu: %s\n", *pos, token_print(tokens->tokens[*pos]));
    return tokens->tokens[*pos];
}

bool accept(huo_ast **ret, size_t *pos, struct Tokens *tokens, parse_func *to_accept) {
    size_t c_pos = *pos;
    huo_ast *temp = to_accept(pos, tokens);
    if (temp == NULL) {
        *pos = c_pos;
        //ret = ret; // Needed or else ret is considered uninit, and as such
        return false;
    } else {
        *ret = temp;
        //*pos = *pos;
        return true;
    }
}

bool peek_token(size_t *pos, struct Tokens *tokens, enum token_type_t token) {
    return (get_token(pos, tokens).type == token);
}

struct Token next_token(size_t *pos, struct Tokens *tokens) {
    return tokens->tokens[(*pos)++];
}

bool discard_token(size_t *pos, struct Tokens *tokens, enum token_type_t token) {
    if (get_token(pos, tokens).type == token) {
        (*pos)++;
        return true;
    } else {
        return false;
    }
}

bool discard(size_t *pos, struct Tokens *tokens, parse_func *to_accept) {
    huo_ast *ret = NULL;
    bool b = accept(&ret, pos, tokens, to_accept);
    if (b) {
        free_if_nonnull(ret);
    }
    return b;
}

bool discard_any(size_t *pos, struct Tokens *tokens, size_t num_to_accept, parse_func **to_accept) {
    for (size_t i = 0; i < num_to_accept; i++) {
        if (discard(pos, tokens, to_accept[i])) {
            return true;
        }
    }
    return false;
}

bool accept_any(huo_ast **ret, size_t *pos, struct Tokens *tokens, size_t num_to_accept, parse_func **to_accept) {
    for (size_t i = 0; i < num_to_accept; i++) {
        if (accept(ret, pos, tokens, to_accept[i])) {
            return true;
        }
    }
    return false;
}

bool discard_whitespace(size_t *pos, struct Tokens *tokens) {
    bool discarded = false;
    while (    discard_token(pos, tokens, TOK_WHITESPACE)
            || discard_token(pos, tokens, TOK_NEWLINE)
            || discard_token(pos, tokens, TOK_COMMENT)) {
            discarded = true;
    }
    return discarded;
}

huo_ast *parse(struct Tokens *tokens) {
    //for (size_t i = 0; i < tokens->length; i++) {
    //    printf("%zu: %s\n", i, token_print(get_token(&i, tokens)));
    //}
    size_t pos = 0;
    huo_ast *ast = NULL;
    discard_whitespace(&pos, tokens);
    if (accept(&ast, &pos, tokens, parse_main)) {

    } else if (accept(&ast, &pos, tokens, parse_open_bracket)) {

    } else if (accept(&ast, &pos, tokens, parse_open_square)) {

    } else {
        ERROR("Invalid parse!");
    }
    discard_whitespace(&pos, tokens);
    EXPECT_TOKEN(&pos, tokens, TOK_EOF);
    return ast;
}

huo_ast *parse_main(size_t *pos, struct Tokens *tokens) {
    huo_ast *tree = ast_new(AST_STATEMENT, value_from_undef());

    while (true) {
        discard_whitespace(pos, tokens);
        huo_ast *child = NULL;
        if (peek_token(pos, tokens, TOK_EOF)) {
            break;
        } else if (peek_token(pos, tokens, TOK_CLOSE_BRACKET)) {
            break;
        } else if (accept(&child, pos, tokens, parse_statement)) {
            ast_push_child(tree, child);
        } else {
            ERROR("Unknown token: %s", token_print(get_token(pos, tokens)));
        }
    }
    return tree;
}

huo_ast *parse_open_bracket(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);

    EXPECT_TOKEN(pos, tokens, TOK_OPEN_BRACKET);

    huo_ast *tree = NULL;

    EXPECT(pos, tokens, &tree, parse_main);

    discard_whitespace(pos, tokens);

    EXPECT_TOKEN(pos, tokens, TOK_CLOSE_BRACKET);

    return tree;

}

huo_ast *parse_statement(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);

    parse_func *norm_children[] = {
            parse_number,
            parse_word,
            parse_open_bracket,
            parse_open_square,
            parse_quote
        };

    huo_ast *child = NULL;
    accept_any(&child, pos, tokens, 5, norm_children);
    return child;
}


huo_ast *parse_float(size_t *pos, struct Tokens *tokens) {
    struct Token int_tok = get_token(pos, tokens);

    struct String int_str;
    if (discard_token(pos, tokens, TOK_NUMBER)) {
        int_str = int_tok.data;
    } else {
        int_str = string_from_char('0');
    }
    EXPECT_TOKEN(pos, tokens, TOK_DOT);
    struct Token frac_tok = get_token(pos, tokens);
    EXPECT_TOKEN(pos, tokens, TOK_NUMBER);
    struct String frac_str = frac_tok.data;

    struct String dot_str = string_from_char('.');
    string_concat_to(&int_str, &dot_str);
    string_concat_to(&int_str, &frac_str);

    char *str = string_to_chars(&int_str);

    float f_val;

    sscanf(str, "%f", &f_val);
    free_if_nonnull(str);

    return ast_new(AST_FLOAT, value_from_float(f_val));
}

huo_ast *parse_int(size_t *pos, struct Tokens *tokens) {
    struct Token t = get_token(pos, tokens);

    EXPECT_TOKEN(pos, tokens, TOK_NUMBER);

    huo_int_t int_val;

    sscanf(string_to_chars(&t.data), "%" SCNhi, &int_val);

    huo_ast *tree = ast_new(AST_INTEGER, value_from_long(int_val));

    assert (get_token(pos, tokens).type != TOK_DOT);

    return tree;
}

huo_ast *parse_number(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);
    bool sign = false;
    bool found_sign = false;
    while (true) {
        if (discard_token(pos, tokens, TOK_MINUS)) {
            sign = !sign;
        } else if (discard_token(pos, tokens, TOK_PLUS)) {
            // pass
        } else {
            break;
        }
        if (found_sign) {
            WARN_ONCE("Multiple signs found!");
        } else {
            found_sign = true;
        }
    }

    huo_ast *tree = NULL;

    // Must parse float before int.
    bool parsed = accept(&tree, pos, tokens, parse_float);

    if (!parsed) {
        parsed = accept(&tree, pos, tokens, parse_int);
    }

    if (parsed && sign) {
        value_negate(ast_value(tree));
    }
    return tree;
}

huo_ast *parse_word(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);

    struct Token t = get_token(pos, tokens);
    //printf("Parsing word at %zu %s", *pos, token_print(t));

    switch (t.type) {
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_AND:
        case TOK_OR:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_EX_MARK:
        case TOK_EQ:
        case TOK_LEFT_ANGLE:
        case TOK_RIGHT_ANGLE:
        case TOK_WORD:
            break;
        case TOK_DOT:
        case TOK_COMMA:
        case TOK_OPEN_BRACKET:
        case TOK_CLOSE_BRACKET:
        case TOK_OPEN_SQUARE:
        case TOK_CLOSE_SQUARE:
        case TOK_NUMBER:
        case TOK_QUOTE:
            return NULL;
        default:
            assert(false);
    }

    next_token(pos, tokens);

    return ast_new(AST_KEYWORD, value_from_keyword(&t.data));
}

huo_ast *parse_quote(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);
    struct Token t = get_token(pos, tokens);

    EXPECT_TOKEN(pos, tokens, TOK_QUOTE);

    struct String str = string_copy_stack(&t.data);

    size_t len = string_length(&str);

    struct Value val = value_from_string(str);

    // Remove quotes
    return ast_new(AST_STRING, substring_ll(1,len - 1, val));
}

huo_ast *parse_array(size_t *pos, struct Tokens *tokens) {

    discard_whitespace(pos, tokens);

    huo_ast *tree = ast_new(AST_ARRAY, value_from_undef());

    while (true) {

        discard_whitespace(pos, tokens);

        huo_ast *child = NULL;

        if (peek_token(pos, tokens, TOK_EOF)) {
            break;
        } else if (peek_token(pos, tokens, TOK_CLOSE_SQUARE)) {
            break;
        } else if (accept(&child, pos, tokens, parse_statement)) {
            ast_push_child(tree, child);
        //} else if (discard_token(pos, tokens, TOK_COMMA)) {
        //    ast_push_child(tree, ast_new(AST_UNDEF, value_from_undef()));
        } else {
            break;
        }

        discard_whitespace(pos, tokens);

        if (peek_token(pos, tokens, TOK_CLOSE_SQUARE)) {
            break;
        }

        EXPECT_TOKEN(pos, tokens, TOK_COMMA);
    }
    return tree;
}

huo_ast *parse_open_square(size_t *pos, struct Tokens *tokens) {
    discard_whitespace(pos, tokens);
    EXPECT_TOKEN(pos, tokens, TOK_OPEN_SQUARE);

    huo_ast *tree = NULL;

    EXPECT(pos, tokens, &tree, parse_array);
    discard_whitespace(pos, tokens);
    discard_token(pos, tokens, TOK_COMMA); // Allow trailing comma.
    discard_whitespace(pos, tokens);

    EXPECT_TOKEN(pos, tokens, TOK_CLOSE_SQUARE);
    return tree;
}

