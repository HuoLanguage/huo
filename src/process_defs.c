#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "core_functions.h"
#include "process_defs.h"
#include "execute.h"
#include "execution_functions/let_binding.h"
#include "structures/hash_table.h"
#include "base_util.h"
#include "config.h"

void make_args_map(struct Execution_bundle * exec_bundle, huo_ast * function) {
    huo_ast * ast = exec_bundle->ast;
    // we want to evaluate the values passed into the function
    // but store the result in the next scope, not the current one
    if (ast_size(function) != ast_size(ast) + 2) {
        ERROR("Wrong number of arguments!: %zu != %zu", ast_size(function) < 2 ? 0 : ast_size(function) - 2, ast_size(ast));
    }
    struct Value vals[ast_size(ast)];
    for(size_t i = 0; i + 1 < ast_size(ast); i++){
        exec_bundle->ast = ast_child(ast, i+1);
        vals[i] = execute(exec_bundle);
    }
    exec_bundle->ast = ast;
    push_scope(exec_bundle->scopes);
    for(size_t i = 0; i + 1 < ast_size(ast); i++){
        char t = ast_value(ast_child(function, i + 2))->type;
        if (t != TYPE_KEYWORD) {
            ERROR("Invalid type for argument: '%c' != TYPE_KEYWORD", t);
        }
        store_let_value(ast_value(ast_child(function, i + 2)), &vals[i], exec_bundle->scopes);
    }
}

huo_ast * get_defined_body(huo_ast * function){
    // just pulls the function body out of a (def...)
    if (ast_size(function) <= 1) {
        ERROR("No function body!");
    }
    size_t index = ast_size(function) - 1;
    huo_ast *child = ast_child(function, index);
    if (ast_size(child) == 0) {
        ERROR("No function body!");
    }
    return ast_copy(child);
}

struct definition *get_defined(struct Scopes *scopes, struct String key) {
    //printf("DEFINED:\n");
//    for (size_t i = 0; i <= scopes->current; i++) {
//        for (hash_table_iter *t = hash_table_iter_new(scopes->scopes[i]); hash_table_iter_has_cur(t); hash_table_iter_next(t)) {
//            struct String *k = hash_table_iter_key(t);
//            struct definition *v = hash_table_iter_val(t);
//             if (v->is_func) {
//                 printf("%s -> func\n", string_to_chars(k));
//             } else {
//                 printf("%s -> '", string_to_chars(k));
//                 print(*(v->val.val));
//                 printf("'\n");
//             }
//        }
//    }
    //printf("'%s'\n", string_to_chars(&key));
    assert(string_is_sane(&key));
    for (size_t ipo = scopes->current + 1; ipo > 0; ipo--) {
        size_t i = ipo - 1;
        hash_table *current_scope = scopes->scopes[i];
        struct definition *def = (struct definition *) hash_table_get(current_scope, &key);
        if (def != NULL) {
            return def;
        }
    }
    return NULL;
}

huo_ast *get_defined_func(struct Scopes *defined, struct String key) {
//     printf("DEF '%s'\n", string_to_chars(&key));
    struct definition *def = get_defined(defined, key);
    if(def == NULL || !def->is_func) {
//         printf("FALSE\n");
        return NULL;
    } else {
//         printf("TRUE\n");
        return def->val.ast;
    }
}

struct Value *get_letted_value(struct Scopes *defined, struct String key) {
//     printf("LET '%s'\n", string_to_chars(&key));
    struct definition *def = get_defined(defined, key);
    if(def == NULL || def->is_func) {
//         printf("FALSE\n");
        return NULL;
    } else {
//         printf("TRUE\n");
        return def->val.val;
    }
}
