#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures/structures.h"
#include "constants.h"
#include "core_functions.h"
#include "build_array.h"
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
huo_ast *parse_statements(size_t *pos, struct Tokens *tokens);
huo_ast *parse_float(size_t *pos, struct Tokens *tokens);
huo_ast *parse_int(size_t *pos, struct Tokens *tokens);
huo_ast *parse_number(size_t *pos, struct Tokens *tokens);
huo_ast *parse_word(size_t *pos, struct Tokens *tokens);
huo_ast *parse_quote(size_t *pos, struct Tokens *tokens);
huo_ast *parse_open_square(size_t *pos, struct Tokens *tokens);

#define EXPECT(to, x) do {if (!accept((to), pos, tokens, (x))) {return NULL;}} while (0)
#define EXPECT_TOKEN(x) do {if (get_token(pos, tokens).type != (x)) {return NULL;} (*pos) += 1;} while (0)

void tree_add_child(huo_ast *tree, huo_ast *child) {
    RESIZE(tree->children, tree->size+1);
    tree->children[tree->size] = child;
    child->parent = tree;
    tree->size += 1;
}

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

huo_ast *parse_whitespace(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(WHITESPACE);
    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_newline(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(NEWLINE);
    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_comment(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(COMMENT);
    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

bool discard_whitespace(size_t *pos, struct Tokens *tokens) {
    bool discarded = false;
    parse_func *ignored[] = {parse_whitespace, parse_newline, parse_comment};
    while (discard_any(pos, tokens, 3, ignored)) {
        discarded = true;
    }
    return discarded;
}

huo_ast *parse_eof(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(T_EOF);
    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_comma(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(COMMA);
    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse(struct Tokens *tokens) {
    size_t pos = 0;
    return parse_main(&pos, tokens);
}

huo_ast *parse_main(size_t *pos, struct Tokens *tokens) {

    huo_ast *tree = malloc_or_die(sizeof(huo_ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    huo_ast *child = NULL;

    if (accept(&child, pos, tokens, parse_open_bracket_internals)) {
        free_if_nonnull(tree);
        return child;
    }
    while (true){

        discard_whitespace(pos, tokens);

        if (discard(pos, tokens, parse_eof)) {
            return tree;
        } else if (discard(pos, tokens, parse_comment)) {
            // pass
        } else if (accept(&child, pos, tokens, parse_open_bracket)) {
            tree_add_child(tree, child);
        } else {
            ERROR("Could not parse token %s", token_print(tokens->tokens[*pos]));
        }
    }
}

huo_ast *parse_function(size_t *pos, struct Tokens *tokens) {

    discard_whitespace(pos, tokens);

    struct Token t = get_token(pos, tokens);
    switch (t.type) {
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case EX_MARK:
        case EQ:
        case LEFT_ANGLE:
        case RIGHT_ANGLE:
        case WORD:
            break;
        default:
            return NULL;
    }
    (*pos) += 1;

    huo_ast *args = ast_new(FUNCTION_ARGS, value_from_undef());
    huo_ast *child;

    discard_whitespace(pos, tokens);
    while (accept(&child, pos, tokens, parse_word)) {
        huo_push_child(args, child);
        discard_whitespace(pos, tokens);
    }

    if (get_token(pos, tokens).type == CLOSE_BRACKET) {
        // Function body is a keyword too!
        // So pop an argument from the function and move it to the body instead.
        child = tree_pop_child(args);
    } else {
        EXPECT(&child, parse_statement);
    }
    huo_ast *tree = ast_new(FUNCTION, value_from_keyword(&t.data));
    tree_add_child(tree, args);
    tree_add_child(tree, child);

    return tree;
}


huo_ast *parse_open_bracket_internals(size_t *pos, struct Tokens *tokens) {

    huo_ast *tree = NULL;

    if (!accept(&tree, pos, tokens, parse_function)) {
        EXPECT(&tree, parse_statements);
    }

    return tree;
}

huo_ast *parse_open_bracket(size_t *pos, struct Tokens *tokens) {

    EXPECT_TOKEN(OPEN_BRACKET);

    huo_ast *tree = NULL;

    EXPECT(&tree, parse_open_bracket_internals);

    EXPECT_TOKEN(CLOSE_BRACKET);

    return tree;

}

huo_ast *parse_statement(size_t *pos, struct Tokens *tokens) {
    parse_func *norm_children[] = {
            parse_number,
            parse_word,
            parse_open_bracket,
            parse_open_square,
            parse_quote
        };

    huo_ast *child = NULL;
    accept_any(child, pos, tokens, norm_children);
    return child;
}

huo_ast *parse_statements(size_t *pos, struct Tokens *tokens) {

    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    while (true) {

        discard_whitespace(pos, tokens);

        huo_ast *child = NULL;

        if (discard(pos, tokens, parse_eof)) {
            ERROR("Unbalanced bracket!");
        } else if (accept(&child, pos, tokens, parse_statement)) {
            tree_add_child(tree, child);
        } else {
            break;
        }
    }
    printf("Ending %zu\n", *pos);
    return tree;
}

huo_ast *parse_float(size_t *pos, struct Tokens *tokens) {

    struct String int_str = get_token(pos, tokens).data;
    EXPECT_TOKEN(NUMBER);
    EXPECT_TOKEN(DOT);
    struct String frac_str = get_token(pos, tokens).data;
    EXPECT_TOKEN(NUMBER);

    struct String dot_str = string_from_char('.');
    string_concat_to(&int_str, &dot_str);
    string_concat_to(&int_str, &frac_str);

    char *str = string_to_chars(&int_str);

    float f_val;

    sscanf(str, "%f", &f_val);
    free_if_nonnull(str);

    huo_ast *tree = malloc_or_die(sizeof(ast));
    tree->content = value_from_float(f_val);
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;
    return tree;
}

huo_ast *parse_int(size_t *pos, struct Tokens *tokens) {
    struct Token t = get_token(pos, tokens);

    EXPECT_TOKEN(NUMBER);

    huo_int_t int_val;

    sscanf(string_to_chars(&t.data), "%" SCNhi, &int_val);

    huo_ast *tree = malloc_or_die(sizeof(ast));
    tree->content = value_from_long(int_val);
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_number(size_t *pos, struct Tokens *tokens) {
    bool sign = false;
    while (true) {
        if (discard_token(pos, tokens, MINUS)) {
            sign = !sign;
        } else if (discard_token(pos, tokens, PLUS)) {
            // pass
        } else {
            break;
        }
    }

    huo_ast *tree = NULL;

    if (accept(&tree, pos, tokens, parse_float) ||
        accept(&tree, pos, tokens, parse_int)) {
        if (sign) {
            value_negate(&tree->content);
        }
    }
    return tree;
}

huo_ast *parse_word(size_t *pos, struct Tokens *tokens) {
    struct Token t = get_token(pos, tokens);

    EXPECT_TOKEN(WORD);

    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_keyword(&t.data);
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_quote(size_t *pos, struct Tokens *tokens) {
    struct Token t = get_token(pos, tokens);

    EXPECT_TOKEN(QUOTE);

    huo_ast *tree = malloc_or_die(sizeof(ast));

    struct String str = string_copy_stack(&t.data);

    size_t len = string_length(&str);

    struct Value val = value_from_string(str);

    // Remove quotes
    tree->content = substring_ll(1,len - 1, val);;
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    return tree;
}

huo_ast *parse_array(size_t *pos, struct Tokens *tokens) {

    huo_ast *tree = malloc_or_die(sizeof(ast));

    tree->content = value_from_undef();
    tree->parent = NULL;
    tree->children = NULL;
    tree->size = 0;

    while (true) {

        discard_whitespace(pos, tokens);

        huo_ast *child = NULL;

        if (discard(pos, tokens, parse_eof)) {
            ERROR("Unbalanced bracket!");
        } else if (accept(&child, pos, tokens, parse_statement)) {
            tree_add_child(tree, child);
        } else {
            break;
        }

        discard_whitespace(pos, tokens);

        EXPECT(&child, parse_comma);
    }
    return tree;
}

huo_ast *parse_open_square(size_t *pos, struct Tokens *tokens) {
    EXPECT_TOKEN(OPEN_SQUARE);

    huo_ast *tree = NULL;

    EXPECT(&tree, parse_array);

    EXPECT_TOKEN(CLOSE_SQUARE);
    return tree;
}

