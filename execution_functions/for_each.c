#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "let_binding.h"
#include "for_each.h"

struct Value for_each(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes){
    if (ast->size < 4) {
        ERROR("Not enough arguments for for_each: %i < 4\n", ast->size);
    }
    struct Value array = execute(ast->children[0], defined, scopes);
    if(array.type == 's'){
        return for_each_string(array, ast, defined, scopes);
    }
    for(int i = 0; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        struct Value index = {
            .type = 'l',
            .data = {
                .ln=(long)i
            }
        };
        struct Tree * function = duplicate_tree(ast->children[3]);
        store_let_value(&ast->children[1]->content, item, scopes);
        store_let_value(&ast->children[2]->content, &index, scopes);
        execute(function, defined, scopes);
    }
    return array;
}

struct Value for_each_string(struct Value string, struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes){
    for(int i = 0; i < string.data.str.length; i++){
        struct Value item = substring(i, i+1, string);
        struct Value index = {
            .type = 'l',
            .data = {
                .ln=(long)i
            }
        };
        struct Tree * function = duplicate_tree(ast->children[3]);
        store_let_value(&ast->children[1]->content, &item, scopes);
        store_let_value(&ast->children[2]->content, &index, scopes);
        execute(function, defined, scopes);
    }
    return string;
}
