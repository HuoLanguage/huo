#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "populate_reduce_function.h"
#include "../core_functions.h"

struct Value reduce_array(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 4) {
        ERROR("Not enough arguments for reduce_array: %i < 4\n", ast->size);
    }
    if (ast->children[1]->type != 'k') {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast->children[1]->type);
    }
    if (ast->children[2]->type != 'k') {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast->children[2]->type);
    }
    int start = 0;
    struct Value result;
    struct Value array = execute(ast->children[0], defined, let_map, max_depth - 1);
    if (array.type != ARRAY) {
        ERROR("Invalid type for reduce_array: '%c' != ARRAY\n", array.type);
    }
    if (array.data.array->size <= 0) {
        return copy_value_stack(&array);
    }
    if(ast->size == 5){
        result = execute(ast->children[4], defined, let_map, max_depth - 1);
    } else {
        result = * array.data.array->values[0];
        start = 1;
    }
    for(int i = start; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        struct Tree * function = duplicate_tree(ast->children[3]);

        populate_reduce_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &result);
        result = execute(function, defined, let_map, max_depth - 1);
    }
    return result;
}
