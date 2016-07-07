#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../config.h"

struct Value map_array(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, huo_depth_t max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    bool use_index;
    size_t func_index;
    if (ast->size == 4) {
        use_index = true;
        func_index = 3;
    } else if (ast->size == 3) {
        use_index = false;
        func_index = 2;
    } else {
        ERROR("Wrong number of arguments for map__array: %zu != [3,4]\n", ast->size);
    }
    struct Value array = execute(ast->children[0], defined, scopes, function_names, max_depth - 1);
    if (array.type != ARRAY) {
        ERROR("Wrong type for map: '%c' != ARRAY", array.type);
    }
    for(size_t i = 0; i < array.data.array->size; i++){
        struct Value *item = value_copy_heap(array.data.array->values[i]);
        struct Tree * function = duplicate_tree(ast->children[func_index]);
        store_let_value(&ast->children[1]->content, item, scopes);
        if (use_index) {
            struct Value index = value_from_long(i);
            store_let_value(&ast->children[2]->content, &index, scopes);
        }
        struct Value result = execute(function, defined, scopes, function_names, max_depth - 1);
        array.data.array->values[i] = value_copy_heap(&result);
    }
    return array;
}
