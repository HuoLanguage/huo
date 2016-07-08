#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "base_util.h"
#include "core_functions.h"
#include "process_defs.h"
#include "execute.h"
#include "execution_functions/let_binding.h"
#include "structures/hash_table.h"
#include "config.h"

void make_args_map(struct Execution_bundle * exec_bundle, struct Tree * function) {
    struct Tree * ast = exec_bundle->ast;
    // we want to evaluate the values passed into the function
    // but store the result in the next scope, not the current one
    if (function->size != ast->size + 2) {
        ERROR("Wrong number of arguments!: %zu != %zu", function->size - 2, ast->size);
    }
    struct Value vals[ast->size];
    for(size_t i = 0; i < ast->size; i++){
        exec_bundle->ast = ast->children[i];
        vals[i] = execute(exec_bundle);
    }
    make_scope(exec_bundle->scopes);
    for(size_t l = 0; l < ast->size; l++){
        char t = function->children[l+1]->content.type;
        if (t != KEYWORD) {
            ERROR("Invalid type for argument: '%c' != KEYWORD", t);
        }
        store_let_value(&function->children[l+1]->content, &vals[l], exec_bundle->scopes);
    }
}

struct Tree * get_defined_body(struct Tree * function){
    // just pulls the function body out of a (def...)
    if (function->size <= 1) {
        ERROR("No function body!");
    }
    size_t index = function->size - 1;
    if (function->children[index]->size == 0) {
        ERROR("No function body!");
    }
    return function->children[index];
}

struct Tree *get_defined_func(hash_table *defined, struct String key) {
    assert(string_is_sane(&key));
    return (struct Tree *) hash_table_get(defined, &key);
}
