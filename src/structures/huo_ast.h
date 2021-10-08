#ifndef AST_H
#define AST_H 1

#include <stdbool.h>
#include <stddef.h>

typedef struct huo_ast_t huo_ast;

#include "value.h"

enum ast_type_e {
    AST_STATEMENT,
    AST_ARRAY,
    AST_FLOAT,
    AST_INTEGER,
    AST_BOOLEAN,
    AST_STRING,
    AST_KEYWORD,
};

huo_ast * value_as_ast(struct Value *v);
struct Value value_from_ast(huo_ast *ast);

huo_ast *ast_new(enum ast_type_e type, struct Value v);
enum ast_type_e ast_type(huo_ast *const tree);
struct Value *ast_value(huo_ast *const tree);
void ast_set_value(huo_ast *tree, struct Value val);
void ast_push_child(huo_ast *tree, huo_ast *child);
huo_ast *ast_pop_child(huo_ast *tree);
huo_ast *ast_child(huo_ast *tree, size_t i);
void ast_set_child(huo_ast *tree, size_t i, huo_ast *child);
size_t ast_size(huo_ast *const tree);
struct String *ast_to_string(huo_ast *const tree);

huo_ast *ast_copy(huo_ast *const from);
void ast_copy_to(huo_ast *to, huo_ast *const from);

void ast_free(huo_ast *ast);

#endif
