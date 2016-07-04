#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../config.h"

struct Value map_array(struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 4) {
        ERROR("Not enough arguments for map_array: %i < 4\n", ast->size);
    }
    struct Value array = execute(ast->children[0], defined, scopes, max_depth - 1);
    if (array.type != ARRAY) {
        ERROR("Wrong type for map: '%c' != ARRAY", array.type);
    }
    for(int i = 0; i < array.data.array->size; i++){
        struct Value *item = value_copy_heap(array.data.array->values[i]);
        struct Value index = value_from_long(i);
        struct Tree * function = duplicate_tree(ast->children[3]);
        store_let_value(&ast->children[1]->content, item, scopes);
        store_let_value(&ast->children[2]->content, &index, scopes);
        struct Value result = execute(function, defined, scopes, max_depth - 1);
        array.data.array->values[i] = value_copy_heap(&result);
    }
    return array;
}
