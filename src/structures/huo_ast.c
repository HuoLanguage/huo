#include <stdlib.h>
#include <string.h>

#define GENERATE_AST_TYPE_ENUM_FUNCS 1
#include "huo_ast.h"
#undef GENERATE_AST_TYPE_ENUM_FUNCS

#include "../base_util.h"


struct huo_ast_t {
    enum ast_type_e type;
    size_t size;
    struct Value content;
    huo_ast ** children;
};

huo_ast *ast_new(enum ast_type_e type, struct Value v) {
    struct huo_ast_t *tree = malloc_or_die(sizeof(struct huo_ast_t));
    tree->type = type;
    tree->content = v;
    tree->children = NULL;
    return tree;
}
enum ast_type_e ast_type(huo_ast *const tree) {
    assert (tree != NULL);
    return tree->type;
}
struct Value *ast_value(huo_ast *const tree) {
    assert (tree != NULL);
    return &tree->content;
}
void ast_set_value(huo_ast *tree, struct Value val) {
    tree->content = val;
}
void ast_push_child(huo_ast *tree, huo_ast *child) {
    assert (tree != NULL);
    assert (child != NULL);
    RESIZE(tree->children, tree->size + 1);
    tree->children[tree->size++] = child;
}
huo_ast *ast_pop_child(huo_ast *tree) {
    assert (tree != NULL);
    assert (ast_size(tree) != 0);
    huo_ast *child = ast_child(tree, ast_size(tree) - 1);
    RESIZE(tree->children, tree->size - 1);
    return child;
}
huo_ast *ast_child(huo_ast *tree, size_t i) {
    assert (i < ast_size(tree));
    return tree->children[i];
}
size_t ast_size(huo_ast *const tree) {
    return tree->size;
}

struct String ast_to_string_indent(huo_ast *const tree, size_t indent);

struct String ast_print_wrap(huo_ast *const tree, size_t indent, char *start, char *end) {
    // Formats like this:

//  ("a"
//   2
//   3
//   "qwert")

//  or
//  ( )

    struct String s = string_from_chars(start);
    if (ast_size(tree) > 0) {
        string_concat_to_consume(&s, ast_to_string_indent(ast_child(tree, 0), 0));
        for (size_t i = 1; i < ast_size(tree); i++) {
            huo_ast *child = ast_child(tree, i);
            string_concat_to_chars(&s, "\n");
            string_concat_to_consume(&s, ast_to_string_indent(child, indent + 1));
        }
    } else {
        string_concat_to_chars(&s, " ");
    }
    string_concat_to_chars(&s, end);
    return s;
}

struct String ast_to_string_indent(huo_ast *const tree, size_t indent) {
    char *indent_str = ARR_MALLOC(indent+1, char);
    memset(indent_str, ' ', indent);
    indent_str[indent] = 0;

    struct String s = string_from_chars("");
    bool last_child = false;
    switch (tree->type) {
        case FUNCTION_DEF:
            string_concat_to_chars(&s, "(def ");
            string_concat_to_consume(&s, value_as_keyword(ast_value(tree)));
            string_concat_to_chars(&s, " ");
            string_concat_to_consume(&s, ast_to_string_indent(ast_child(tree, 0), 0));
            string_concat_to_chars(&s, "\n");
            string_concat_to_consume(&s, ast_to_string_indent(ast_child(tree, 1), indent + 4));
            string_concat_to_chars(&s, ")");
            break;
        case FUNCTION_APP:
            string_concat_to_chars(&s, "(");
            string_concat_to_consume(&s, value_as_keyword(ast_value(tree)));
            string_concat_to_chars(&s, " ");
            string_concat_to_consume(&s, ast_to_string_indent(ast_child(tree, 0), 0));
            string_concat_to_chars(&s, "\n");
            string_concat_to_consume(&s, ast_to_string_indent(ast_child(tree, 1), indent + 4));
            string_concat_to_chars(&s, ")");
            break;
        case FUNCTION_ARGS:
            for (size_t i = 0; i < ast_size(tree); i++) {
                huo_ast *child = ast_child(tree, i);
                string_concat_to_consume(&s, ast_to_string_indent(child, 0));
                last_child = ast_size(tree) == 0;
                if (!last_child) {
                    string_concat_to_chars(&s, " ");
                }
            }
            break;
        case FUNCTION_BODY:
            string_concat_to_consume(&s, ast_print_wrap(tree, indent, "", ""));
        case STATEMENT:
            string_concat_to_consume(&s, ast_print_wrap(tree, indent, "(", ")"));
            break;
        case ARRAY:
            string_concat_to_consume(&s, ast_print_wrap(tree, indent, "[", "]"));
            break;
        case STATEMENTS:
            for (size_t i = 0; i < ast_size(tree); i++) {
                huo_ast *child = ast_child(tree, i);
                string_concat_to_consume(&s, ast_to_string_indent(child, indent));
                last_child = ast_size(tree) == 0;
                if (!last_child) {
                    string_concat_to_chars(&s, "\n");
                }
            }
            break;
        case FLOAT:
            string_concat_to_float(&s, value_as_float(ast_value(tree)));
            break;
        case INTEGER:
            string_concat_to_long(&s, value_as_float(ast_value(tree)));
            break;
        case AST_WORD:
            string_concat_to_consume(&s, value_as_keyword(ast_value(tree)));
            break;
        case STRING:
            string_concat_to_chars(&s, "\"");
            string_concat_to_consume(&s, value_as_string(ast_value(tree)));
            string_concat_to_chars(&s, "\"");
            break;
    }

    free(indent_str);
    return s;
}
struct String ast_to_string(huo_ast *const tree) {
    return ast_to_string_indent(tree, 0);
}


huo_ast *ast_copy(huo_ast *const from) {
    huo_ast *ast = malloc_or_die(sizeof(struct huo_ast_t));
    ast->type = from->type;
    ast->content = value_copy_stack(&from->content);
    ast->children = NULL;
    ast->size = from->size;
    RESIZE(ast->children, from->size);
    for(size_t i = 0; i < from->size; i++){
        ast->children[i] = ast_copy(from->children[i]);
    }
    return ast;
}

void ast_copy_to(huo_ast *to, huo_ast *const from) {
    to->type = from->type;
    value_copy_to(&to->content, &from->content);

    for(size_t i = to->size; i < from->size; i++){
        ast_free(to->children[i]);
    }

    RESIZE(to->children, from->size);
    to->size = from->size;
    for(size_t i = 0; i < from->size; i++){
        ast_copy_to(to->children[i], from->children[i]);
    }

}

void ast_free(huo_ast *ast) {
    if (ast != NULL) {
        value_free_stack(ast->content);
        if (ast->children != NULL) {
            for(size_t i = 0; i < ast->size; i++){
                ast_free(ast->children[i]);
            }
            free(ast->children);
        }
        free(ast);
    }
}
