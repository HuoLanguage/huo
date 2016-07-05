#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../core_functions.h"
#include "../config.h"

struct Value reduce_array(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size != 4 && ast->size != 5) {
        ERROR("Wrong number of arguments for reduce_array: %zu != [4, 5]\n", ast->size);
    }
    if (ast->children[1]->type != 'k') {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast->children[1]->type);
    }
    if (ast->children[2]->type != 'k') {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast->children[2]->type);
    }
    size_t start;
    struct Value result;
    struct Value array = execute(ast->children[0], defined, scopes, max_depth - 1);
    if (array.type != ARRAY) {
        ERROR("Invalid type for reduce_array: '%c' != ARRAY\n", array.type);
    }
    if (array.data.array->size <= 0) {
        return value_copy_stack(&array);
    }
    if(ast->size == 5){
        result = execute(ast->children[4], defined, scopes, max_depth - 1);
        start = 0;
    } else {
        result = *array.data.array->values[0];
        start = 1;
    }
    for(size_t i = start; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        struct Tree * function = duplicate_tree(ast->children[3]);
        store_let_value(&ast->children[1]->content, &result, scopes);
        store_let_value(&ast->children[2]->content, item, scopes);
        result = execute(function, defined, scopes, max_depth - 1);
    }
    return result;
}
